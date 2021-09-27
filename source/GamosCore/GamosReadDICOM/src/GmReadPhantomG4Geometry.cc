#include "GmReadPhantomG4Geometry.hh"
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
#include "G4PhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmReadPhantomG4Geometry::GmReadPhantomG4Geometry()
{
  //  thePhantomFileName = "test.g4dcm";
}

//---------------------------------------------------------------------------
GmReadPhantomG4Geometry::~GmReadPhantomG4Geometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomG4Geometry::ReadPhantomData()
{
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", thePhantomFileName);

  filename = GmGenUtils::FileInPath( filename );

  GmFileIn fing = GmFileIn::GetInstance(filename);
  std::ifstream* fin = fing.GetIfstream();
  std::vector<G4String> wl;
  G4int nMaterials;
  if( !fing.GetWordsInLine(wl) ) return;
  nMaterials = GmGenUtils::GetInteger(wl[0]);
  G4String stemp;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  G4int nmate;
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    if( !fing.GetWordsInLine(wl) ) return;
    nmate = GmGenUtils::GetInteger(wl[0]);
    stemp = wl[1];
    //    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData reading nmate " << ii << " = " << nmate << " mate " << stemp << G4endl; //GDEB
    if( ii != nmate ) G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
				  "Material number should be in increasing order: wrong material number ");
    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  *fin >> thePatientPosition;
  if( GmGenUtils::IsNumber( thePatientPosition ) ) {
    nVoxelX = G4int(GmGenUtils::GetValue(thePatientPosition));
    *fin >> nVoxelY >> nVoxelZ;
    thePatientPosition = "HFS";
  } else {
    *fin >> nVoxelX >> nVoxelY >> nVoxelZ;
  }
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  *fin >> offsetX >> dimX;
  dimX = (dimX - offsetX)/nVoxelX;
  *fin >> offsetY >> dimY;
  dimY = (dimY - offsetY)/nVoxelY;
  *fin >> offsetZ >> dimZ;
  dimZ = (dimZ - offsetZ)/nVoxelZ;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) {
    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;
  }
#endif
  
  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	*fin >> stemp; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//-	if( ix == 0 && iy == 0 ) G4cout << ix << " " << iy << " " << iz << " filling theMateIDs " << nnew << " = " <<  atoi(stemp.c_str())-1 << " " << stemp << G4endl; //GDEB
	//-	if( ix == 0 && iy == 0 ) G4cout << ix << " " << iy << " " << iz << " filling theMateIDs " << nnew << " = " <<  atoi(stemp.c_str())-1 << " " << stemp << G4endl; //GDEB
	G4int mateID = atoi(stemp.c_str());
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(testVerb) ) {
      G4cout <<"GmReadPhantomG4Geometry: ReadMateID " << nnew << " : " << ix << " : " << iy << " : " << iz << " = " << stemp << " = " << mateID << G4endl;
    }
#endif
    if( mateID < 0 || mateID >= nMaterials ) {
	  G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		      "Wrong index in phantom file",
		      FatalException,
		      G4String("It should be between 0 and "
			       + GmGenUtils::itoa(nMaterials-1) 
			       + ", while it is " 
			       + GmGenUtils::itoa(mateID)).c_str());
	}
	theMateIDs[nnew] = mateID;
      }
    }
  }

  ReadVoxelDensities( *fin );

  ReadPV( fing );

  fin->close();
}
