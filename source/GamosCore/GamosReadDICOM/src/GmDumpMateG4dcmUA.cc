#include "G4Run.hh"
#include "G4Event.hh"

#include "GmDumpMateG4dcmUA.hh"
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomG4Geometry.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVParameterised.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4RunManager.hh"

//-----------------------------------------------------------------------
GmDumpMateG4dcmUA::GmDumpMateG4dcmUA()
{
  //  thePhantomName = GmParameterMgr::GetInstance()->GetStringValue("GmDumpMateG4dcmUA::CrystalName","lyso_crystal");

}

//-----------------------------------------------------------------------
void GmDumpMateG4dcmUA::BeginOfRunAction( const G4Run* )
{
  theOutputType = "DumpMate";

  G4bool phantomRegular = FALSE;
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    G4PhantomParameterisation* paramreg = GmRegularParamUtils::GetInstance()->GetPhantomParam( *cite, FALSE );
    if( paramreg != 0 ){
      if( phantomRegular ) G4Exception("GmDumpMateG4dcmUA::BeginOfRunAction",
				       "Error",
				       FatalException,
				       "Two G4PhantomParameterisation's found ");
      phantomRegular = TRUE;
      thePhantomStructure = *cite;
      theRegularParam = paramreg;
    }
  }

  if( !phantomRegular ) G4Exception("GmDumpMateG4dcmUA::BeginOfRunAction",
				    "Error",
				    FatalException,
				    "No G4PhantomParameterisation found ");

  //  BuildMaterialList();

  PrintImage();
}

/*//-----------------------------------------------------------------------
void GmDumpMateG4dcmUA::BuildMaterialList()
{

const G4MaterialTable* matTab = G4Material::GetMaterialTable();
G4cout << " Number of G4Material's: " << matTab->size() << G4endl;
  std::vector<G4Material*>::const_iterator matite;
  size_t ii = 0;
  for( matite = matTab->begin(); matite != matTab->end(); matite++, ii++ ) {
  theMaterials[*matite] = ii;
  }
  }*/

//-----------------------------------------------------------------------
void GmDumpMateG4dcmUA::PrintImage()
{  

  std::vector<G4Material*> theMaterials = theRegularParam->GetMaterials();
  OpenFileOut();
  // std::ofstream fout(theFileNameOut);
  *theFileOut << theMaterials.size() << G4endl;
  for( size_t ii = 0; ii < theMaterials.size(); ii++ ) {
    *theFileOut << ii << " \"" << theMaterials[ii]->GetName() << "\"" << G4endl;
  }

  /*  std::map< G4Material*, size_t >::const_iterator item; 
  for( item == theMaterials.begin(); item != theMaterials.end(); item++ ) {
    *theFileOut << (*item).second << " " << (*item).first->GetName() << G4endl;
    } */

  size_t nX, nY, nZ;
  nX = theRegularParam->GetNoVoxelsX();
  nY = theRegularParam->GetNoVoxelsY();
  nZ = theRegularParam->GetNoVoxelsZ();
  G4double vHalfX = theRegularParam->GetVoxelHalfX();
  G4double vHalfY = theRegularParam->GetVoxelHalfY();
  G4double vHalfZ = theRegularParam->GetVoxelHalfZ();
  
/*  G4ThreeVector transl = theRegularParam->GetTranslation();
  G4double minX = trans.x()-nX*vHalfX;
  G4double maxX = trans.x()+nX*vHalfX;
  G4double minY = trans.y()-nY*vHalfY;
  G4double maxY = trans.y()+nY*vHalfY;
  G4double minZ = trans.z()-nZ*vHalfZ;
  G4double maxZ = trans.z()+nZ*vHalfZ;
*/
  G4VSolid* phantomCont = theRegularParam->GetContainerSolid();
std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( phantomCont->GetName() );
G4ThreeVector transl = touchs[0]->GetGlobalPosition();
G4cout << " PHANTOM CONTAINER GLOBAL POS " << transl << G4endl; //GDEB

G4String patientPosition = "HFS";
const GmReadPhantomG4Geometry* g4geom = dynamic_cast<const GmReadPhantomG4Geometry*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
if( g4geom ) {
patientPosition = g4geom->GetPatientPosition();
}
 G4double minX;
 G4double maxX;
 G4double minY;
 G4double maxY;
 G4double minZ;
 G4double maxZ;
if( patientPosition == "HFS" ) {
  minX = transl.x()-nX*vHalfX;
  maxX = transl.x()+nX*vHalfX;
  minY = transl.z()-nY*vHalfY;
  maxY = transl.z()+nY*vHalfY;
  minZ = -transl.y()-nZ*vHalfZ;
  maxZ = -transl.y()+nZ*vHalfZ;
 } else {
  G4Exception("GDumpMateG4dcmUA::BeginOfRunAction",
	      "",
	      FatalException,
	      ("Only patient position HFS is supported, while you are using " + patientPosition + "   Please contact GAMOS experts").c_str());
 }

  /*
    G4ThreeVector thePhantomMinusCorner = (theRegularParam->GetTranslation(0) + theRegularParam->GetTranslation(theRegularParam->GetNoVoxels()-1) )/ 2;
    G4cout << " phantom minus corner " << thePhantomMinusCorner << " " << theRegularParam->GetTranslation(0) << " " << theRegularParam->GetTranslation(theRegularParam->GetNoVoxels()-1) << G4endl;
    G4double minX = thePhantomMinusCorner.x();
    G4double maxX = thePhantomMinusCorner.x()+2.*vHalfX*nX;
    G4double minY = thePhantomMinusCorner.y();
    G4double maxY = thePhantomMinusCorner.y()+2.*vHalfY*nY;
    G4double minZ = thePhantomMinusCorner.z();
    G4double maxZ = thePhantomMinusCorner.z()+2.*vHalfZ*nZ;
  */

  //--- Dump image header
  *theFileOut << nX << " " << nY << " " << nZ << std::endl;
  *theFileOut << minX << " " << maxX << std::endl;
  *theFileOut << minY << " " << maxY << std::endl;
  *theFileOut << minZ << " " << maxZ << std::endl;

  //--- Dump image material IDs
  G4bool bFloat = false;
  size_t copyNo = 0;
  for( size_t iz = 0; iz < nZ; iz++ ) {
    size_t copyNoXY = 0;
    for( size_t iy = 0; iy < nY; iy++ ) {
      //	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
      for( size_t ix = 0; ix < nX; ix++ ) {
	//	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
	size_t matIdx = theRegularParam->GetMaterialIndex(copyNo);
	if( bFloat ) {
	  *theFileOut << matIdx << " ";
	} else {
	  *theFileOut << std::setprecision(6) << matIdx << " "; // precision to write a size_t (=64 bits)
	}
	copyNo++;
	//-       	if( !bFloat ) *theFileOut << " " << copyNo << " D=" << *data++ << G4endl;
	//	if( ix != nX-1) *theFileOut << " ";
	if( bFloat && copyNoXY%8 == 7 ) *theFileOut << G4endl;
	copyNoXY++;
      }
      if( ( bFloat && copyNo%8 != 0 ) || !bFloat ) *theFileOut << G4endl;
    }
  }

  //--- Dump image material densities
  bFloat = true;
  copyNo = 0;
  for( size_t iz = 0; iz < nZ; iz++ ) {
    size_t copyNoXY = 0;
    for( size_t iy = 0; iy < nY; iy++ ) {
      //	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
      for( size_t ix = 0; ix < nX; ix++ ) {
	//	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
	size_t matIdx = theRegularParam->GetMaterialIndex(copyNo);
	G4double mateDens = theMaterials[matIdx]->GetDensity()/(CLHEP::g/CLHEP::cm3);
	if( bFloat ) {
	  *theFileOut << mateDens << " ";
	} else {
	  *theFileOut << std::setprecision(6) << mateDens << " "; // precision to write a size_t (=64 bits)
	}
	copyNo++;
	//-       	if( !bFloat ) *theFileOut << " " << copyNo << " D=" << *data++ << G4endl;
	//	if( ix != nX-1) *theFileOut << " ";
	if( bFloat && copyNoXY%8 == 7 ) *theFileOut << G4endl;
	copyNoXY++;
      }
      if( ( bFloat && copyNo%8 != 0 ) || !bFloat ) *theFileOut << G4endl;
    }
  }

}
