#include "globals.hh"

#include "GmReadDICOMVerbosity.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "GmSimplePhantomGeometry.hh"
#include "G4PhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmSimplePhantomGeometry::GmSimplePhantomGeometry()
{
  GmGenUtils::itoa(12);
  new G4tgrMessenger;

  parameterisedPhysVolume = 0;
}

//---------------------------------------------------------------------------
GmSimplePhantomGeometry::~GmSimplePhantomGeometry()
{
}


//---------------------------------------------------------------------------
void GmSimplePhantomGeometry::ReadPhantomData()
{

  theMaterialNames = GmParameterMgr::GetInstance()->GetVStringValue("GmSimplePhantomGeometry:MaterialNames",theMaterialNames);
  std::vector<G4double> matez;
  matez = GmParameterMgr::GetInstance()->GetVNumericValue("GmSimplePhantomGeometry:MaterialZVoxels",matez);

  G4int izv = 0;
  for( size_t ii = 0; ii < matez.size(); ii++ ){
    izv += G4int(matez[ii]);
    theMaterialZVoxels[izv] = ii;
    //    G4cout << " theMaterialZVoxels[izv] " << theMaterialZVoxels[izv] << " izv " << izv << " ii " << ii << G4endl;
  }
 
  //  G4cout << " theMaterialZVoxels.size() != theMaterialNames.size() " << theMaterialZVoxels.size() << " " <<  theMaterialNames.size()  << G4endl;
  if( theMaterialZVoxels.size() != theMaterialNames.size() ) {
    G4Exception("GmSimplePhantomGeometry::ReadPhantomData","Wrong number of material Z voxels",FatalErrorInArgument,G4String(G4String("Number of material Z voxels = ") + GmGenUtils::itoa(theMaterialZVoxels.size()) + G4String(" different than number of materials = ") + GmGenUtils::itoa(theMaterialNames.size())).c_str());
  }
 
  theMaterialDensities = GmParameterMgr::GetInstance()->GetVNumericValue("GmSimplePhantomGeometry:MaterialDensities",theMaterialDensities);
  if( theMaterialDensities.size() != theMaterialNames.size() ) {
    G4Exception("GmSimplePhantomGeometry::ReadPhantomData","Wrong number of material densities",FatalErrorInArgument,G4String(G4String("Number of material densities = ") + GmGenUtils::itoa(theMaterialDensities.size()) + G4String(" different than number of materials = ") + GmGenUtils::itoa(theMaterialNames.size())).c_str());
  }
  
  //------- Materials
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 

  for( size_t ii = 0; ii < theMaterialNames.size(); ii++ ){
    G4double density = theMaterialDensities[ii]; // BuildMaterialChangingDensity uses g/cm3 
    //    G4cout << " mate " << ii << " density " << density << G4endl;
    G4Material* mate = matmgr->FindOrBuildG4Material(theMaterialNames[ii]);
    G4String mateName = mate->GetName()+GmGenUtils::ftoa(density);
    if( fabs(density - mate->GetDensity() )/density > 0.001 ) {
      thePhantomMaterials.push_back(BuildMaterialChangingDensity(mate,density,mateName));
    } else {
      thePhantomMaterials.push_back(mate);
    }
    //    G4cout << " MATE " << *(thePhantomMaterials[ii]) << G4endl;
  }

  std::vector<G4double> vdou;
  vdou = GmParameterMgr::GetInstance()->GetVNumericValue("GmSimplePhantomGeometry:NVoxels",vdou);
  if( vdou.size() != 3 ){
    G4cerr << " GmSimplePhantomGeometry:NVoxels must have 3 parameters, it has " << vdou.size() << G4endl;
    G4Exception("GmSimplePhantomGeometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
		"");
}

  nVoxelX = G4int(vdou[0]);
  nVoxelY = G4int(vdou[1]);
  nVoxelZ = G4int(vdou[2]);
  //  G4cout << " nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  if( izv != nVoxelZ ) {
    G4Exception("GmSimplePhantomGeometry::ReadPhantomData",
		"Number of voxels in Z is different that total number of Z materials defined",
		FatalErrorInArgument,
		G4String("Number of voxels in Z = " 
			 + GmGenUtils::itoa(nVoxelZ)
			 + " total number of Z materials = " 
			 + GmGenUtils::itoa(izv)).c_str());
  }


  vdou.clear();
  vdou = GmParameterMgr::GetInstance()->GetVNumericValue("GmSimplePhantomGeometry:PhantomDims",vdou);
  if( vdou.size() != 6 ){
    G4cerr << " GmSimplePhantomGeometry:PhantomDims must have 6 parameters, it has " << vdou.size() << G4endl;
    G4Exception("GmSimplePhantomGeometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }
  offsetX = vdou[0];
  offsetY = vdou[2];
  offsetZ = vdou[4];
  dimX = (vdou[1]-vdou[0])/nVoxelX;
  dimY = (vdou[3]-vdou[2])/nVoxelY;
  dimZ = (vdou[5]-vdou[4])/nVoxelZ;

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ){
    G4cout << "GmSimplePhantomGeometry dimX " << dimX << " offsetX " << offsetX << G4endl;
    G4cout << "GmSimplePhantomGeometry dimY " << dimY << " offsetY " << offsetY << G4endl;
    G4cout << "GmSimplePhantomGeometry dimZ " << dimZ << " offsetZ " << offsetZ << G4endl;
  }
#endif

  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  G4int imate;
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    std::map<G4int,G4int>::iterator ite = theMaterialZVoxels.upper_bound( iz );
    if( ite == theMaterialZVoxels.end() ) {
      imate = theMaterialZVoxels.size()-1;
    } else {
      imate = (*ite).second;
    }
    //    G4cout << " setting mate Z " << iz << " = " << imate << G4endl;
    for( G4int ix = 0; ix < nVoxelX; ix++ ) {
      //      G4cout << " stemp " << stemp << G4endl;
      for( G4int iy = 0; iy < nVoxelY; iy++ ) {
	G4int nnew = iy + (ix)*nVoxelY + (iz)*nVoxelX*nVoxelY;
	
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theMateIDs[nnew] = imate;
      }
    }
  }

}
