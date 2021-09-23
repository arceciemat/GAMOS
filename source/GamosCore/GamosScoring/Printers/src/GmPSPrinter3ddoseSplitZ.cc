#include "GmPSPrinter3ddoseSplitZ.hh"
#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/Gm3ddoseHeader.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

GmPSPrinter3ddoseSplitZ::GmPSPrinter3ddoseSplitZ(G4String name) : GmVPSPrinter( name )
{
  theOutputType = "3ddose";

}

void GmPSPrinter3ddoseSplitZ::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);

  G4String fileName = "3ddose.out";
  G4String scorerName = "";
  if( scorer ) scorerName = scorer->GetName();
  fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+"_"+scorerName+":FileName",fileName);

  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if(suffix != "" ) fileName += suffix;

  G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  if(prefix != "" ) fileName = prefix + fileName;

  OpenFileOut(fileName);

  thePhantomParam = GmGeometryUtils::GetInstance()->GetPhantomParam( TRUE );
  unsigned int nz = thePhantomParam->GetNoVoxelsZ();

  for( unsigned int ii = 0; ii < nz; ii++ ) {  
    Write3ddoseHeader(ii);
    Write3ddose(RunMap, scorer, ii);
  }

}


//-----------------------------------------------------------------------
void GmPSPrinter3ddoseSplitZ::Write3ddoseHeader(unsigned int iz)
{
  Gm3ddoseHeader head;

  //----- Print number of events in origin
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();
  head.SetNumberOfEvents( nev );

  unsigned int nx = thePhantomParam->GetNoVoxelsX();
  unsigned int ny = thePhantomParam->GetNoVoxelsY();
  unsigned int nz = thePhantomParam->GetNoVoxelsZ();
  head.SetNoVoxelsX( nx );
  head.SetNoVoxelsY( ny );
  head.SetNoVoxelsZ( 1 );

  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner = (thePhantomParam->GetTranslation(0) + thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxels()-1) )/ 2;
  G4double voxelX = thePhantomParam->GetVoxelHalfX()*2;
  G4double voxelY = thePhantomParam->GetVoxelHalfY()*2;
  G4double voxelZ = thePhantomParam->GetVoxelHalfZ()*2;
    
  thePhantomMinusCorner -= G4ThreeVector(voxelX*nx/2.,voxelY*ny/2.,voxelZ*nz/2.-voxelZ*iz);

#ifndef GAMOS_NO_VERBOSE
 if( ScoringVerb(infoVerb) ) {
    G4cout << " GmPSPrinter3ddoseSplitZ::Write3ddoseHeader phantom minus corner (temp) " << thePhantomMinusCorner << " " << thePhantomParam->GetTranslation(0) << " " << thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxels()-1) << G4endl;
    G4cout << "GmPSPrinter3ddoseSplitZ::Write3ddoseHeader  voxelX " << thePhantomParam->GetVoxelHalfX() << " = " << voxelX <<  "  " << -voxelX*nx/2. << " " << G4ThreeVector( -nx*voxelX/2.,-ny*voxelY/2.,-voxelZ/2.) << G4endl;
    G4cout << "GmPSPrinter3ddoseSplitZ::Write3ddoseHeader  phantom minus corner " << thePhantomMinusCorner << G4endl;
  }
#endif

   //----- Add coordinates of phantom volume 
  G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );

  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
  GmTouchable* pcont = touchs[0];
  G4ThreeVector phCentre = pcont->GetGlobalPosition();
  //  G4RotationMatrix phRotMat = pcont->GetGlobalRotMat();
  //  phCentre *= phRotMat; // print in the coordinates of the phantom without rotation

  GmReadPhantomGeometry* phGeom = static_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()));
  if( !phGeom ) {
    G4Exception("RTVPlanSource::MoveGeometry",
		"",
		FatalException,
		"Geometry constructor has to be of type GmReadPhantomG4Geometry or one of its derived classes");
  }

  G4double theInitialRotAngleX = phGeom->GetInitialRotAngleX();
  G4double theInitialRotAngleY = phGeom->GetInitialRotAngleY();
  G4double theInitialRotAngleZ = phGeom->GetInitialRotAngleZ();
  G4RotationMatrix phRotMat;
  phRotMat.rotateX(theInitialRotAngleX);
  phRotMat.rotateY(theInitialRotAngleY);
  phRotMat.rotateZ(theInitialRotAngleZ);
  phRotMat.invert();

  phCentre -= phGeom->GetInitialDisp();
  phCentre *= phRotMat; // print in the coordinates of the phantom without rotation
  thePhantomMinusCorner += phCentre;

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPSPrinter3ddoseSplitZ::Write3ddoseHeader phantom minus corner " << thePhantomMinusCorner << *pcont << G4endl;
#endif
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    delete *ite;
  }

  std::vector<float> vval;
  for( unsigned int ii = 0; ii <= nx; ii++ ) {
    vval.push_back( (thePhantomMinusCorner.x() + ii*voxelX) / CLHEP::cm );
    //    *theDoseFile << thePhantomMinusCorner.x() + ii*voxelX << " ";
  }

  head.SetVoxelLimitsX( vval );

  vval.clear();
  //  *theDoseFile << G4endl;
  for( unsigned int ii = 0; ii <= ny; ii++ ) {
    vval.push_back( (thePhantomMinusCorner.y() + ii*voxelY) / CLHEP::cm );
    // *theDoseFile << thePhantomMinusCorner.y() + ii*voxelY << " ";
  }
  //  *theDoseFile << G4endl;
  head.SetVoxelLimitsY( vval );

  vval.clear();
  for( unsigned int ii = 0; ii <= 1; ii++ ) {
    vval.push_back( (thePhantomMinusCorner.z() + ii*voxelZ) / CLHEP::cm );
  }
  head.SetVoxelLimitsZ( vval );

  //  G4cout << head;
  theFileOut = new std::ofstream(theFileNameOut+"_"+GmGenUtils::itoa(iz));

  *theFileOut << head;

}

//-----------------------------------------------------------------------
void GmPSPrinter3ddoseSplitZ::Write3ddose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer, unsigned int iz )
{
  G4int nvoxelsXY = thePhantomParam->GetNoVoxelsX() * thePhantomParam->GetNoVoxelsY();
  G4int nvoxX = thePhantomParam->GetNoVoxelsX();
  
  //----- Loop to all voxels and print dose
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  G4int nvoxelFirst = nvoxelsXY*iz;
  G4int nvoxelLast = nvoxelsXY*(iz+1);
  for( G4int ii = nvoxelFirst; ii < nvoxelLast; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    if( ite != scorerMap->end() ){
      *theFileOut << (*(ite->second))*theUnitRatio;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPSPrinter3ddoseSplitZ::Write3ddose " << ii << " 3ddose " <<  (*(ite->second))*theUnitRatio << " " <<  (*(ite->second)) << " nev " << nev << " unit " << theUnitRatio << G4endl;
#endif
    } else {
      *theFileOut << 0.;
    }
    if( ii%nvoxX == nvoxX-1) {
      *theFileOut << G4endl;
    } else {
      *theFileOut << " ";
    }
    
  }
  
  //----- Loop to all voxels and print dose error
  for( G4int ii = nvoxelFirst; ii < nvoxelLast; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    if( ite != scorerMap->end() ){
      G4double sumX = (*(ite->second));
      G4double error = scorer->GetErrorRelative( ite->first, sumX );
      *theFileOut << error;
    } else {
      *theFileOut << 0.;
    }
    
    if( ii%nvoxX == nvoxX-1) {
      *theFileOut << G4endl;
    } else {
      *theFileOut << " ";
    }
  }

  CloseFileOut();
}
