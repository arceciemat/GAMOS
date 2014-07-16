#include "GmPSPrinterXYZ.hh"
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

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

GmPSPrinterXYZ::GmPSPrinterXYZ(G4String name) : GmVPSPrinter( name )
{
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");
  theOutputType = "3ddose";

}

void GmPSPrinterXYZ::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 0 && params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterXYZ::SetParameters","There should be two optional parameters: UNIT UNIT_NAME",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ){
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << " GmPSPrinterXYZ::SetParameters " << params[0] << " " << theUnit << " " << theUnitName << G4endl;
#endif
  }


}

void GmPSPrinterXYZ::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{ 
  G4String fileName = "XYZdose.out";
  G4String scorerName = "";
  if( theScorer ) scorerName = theScorer->GetName();
  fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+"_"+scorerName+":FileName",fileName);

  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if(suffix != "" ) fileName += suffix;

  G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  if(prefix != "" ) fileName = prefix + fileName;

  OpenFileOut(fileName);

  WriteHeader();
  WriteValues(RunMap, theScorer);

}


//-----------------------------------------------------------------------
void GmPSPrinterXYZ::WriteHeader()
{
  Gm3ddoseHeader head;

  //----- Print number of events in origin
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();
  head.SetNumberOfEvents( nev );

  thePhantomParam = GmRegularParamUtils::GetInstance()->GetPhantomParam( TRUE );

  unsigned int nx = thePhantomParam->GetNoVoxelX();
  unsigned int ny = thePhantomParam->GetNoVoxelY();
  unsigned int nz = thePhantomParam->GetNoVoxelZ();
  head.SetNoVoxelX( nx );
  head.SetNoVoxelY( ny );
  head.SetNoVoxelZ( nz );

  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner = (thePhantomParam->GetTranslation(0) + thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxel()-1) )/ 2;
  G4double voxelX = thePhantomParam->GetVoxelHalfX()*2;
  G4double voxelY = thePhantomParam->GetVoxelHalfY()*2;
  G4double voxelZ = thePhantomParam->GetVoxelHalfZ()*2;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " GmPSPrinterXYZ::Write3ddoseHeader phantom minus corner (temp) " << thePhantomMinusCorner << " " << thePhantomParam->GetTranslation(0) << " " << thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxel()-1) << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPSPrinterXYZ::Write3ddoseHeader  voxelX " << thePhantomParam->GetVoxelHalfX() << " = " << voxelX <<  "  " << -voxelX*nx/2. << " " << G4ThreeVector( -nx*voxelX/2.,-ny*voxelY/2.,-nz*voxelZ/2.) << G4endl;
#endif
  thePhantomMinusCorner -= G4ThreeVector(voxelX*nx/2.,voxelY*ny/2.,voxelZ*nz/2.);
#ifndef GAMOS_NO_VERBOSE
   if( ScoringVerb(infoVerb) ) G4cout << "GmPSPrinterXYZ::Write3ddoseHeader  phantom minus corner " << thePhantomMinusCorner << G4endl;
#endif
   //----- Add coordinates of phantom volume 
  G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );

  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
  GmTouchable* pcont = touchs[0];
  thePhantomMinusCorner += pcont->GetGlobalPosition();
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPSPrinterXYZ::Write3ddoseHeader phantom minus corner " << thePhantomMinusCorner << *pcont << G4endl;
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
  for( unsigned int ii = 0; ii <= nz; ii++ ) {
    vval.push_back( (thePhantomMinusCorner.z() + ii*voxelZ) / CLHEP::cm );
    //  *theDoseFile << thePhantomMinusCorner.z() + ii*voxelZ << " ";
  }
  head.SetVoxelLimitsZ( vval );

  //  G4cout << head;
  *theFileOut << head;

}

//-----------------------------------------------------------------------
void GmPSPrinterXYZ::WriteValues( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* )
{
  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner = (thePhantomParam->GetTranslation(0) + thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxel()-1) )/ 2;
  G4double voxelHalfX = thePhantomParam->GetVoxelHalfX();
  G4double voxelHalfY = thePhantomParam->GetVoxelHalfY();
  G4double voxelHalfZ = thePhantomParam->GetVoxelHalfZ();
  unsigned int nx = thePhantomParam->GetNoVoxelX();
  unsigned int ny = thePhantomParam->GetNoVoxelY();
  unsigned int nz = thePhantomParam->GetNoVoxelZ();
  unsigned int nxy = nx * ny;
  thePhantomMinusCorner -= G4ThreeVector(voxelHalfX*nx,voxelHalfY*ny,voxelHalfZ*nz);

   //----- Add coordinates of phantom volume 
  G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
  GmTouchable* pcont = touchs[0];
  thePhantomMinusCorner += pcont->GetGlobalPosition();
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    delete *ite;
  }

  G4int nvoxels = thePhantomParam->GetNoVoxelX() * thePhantomParam->GetNoVoxelY() * thePhantomParam->GetNoVoxelZ();
  
  //----- Loop to all voxels and print dose
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    unsigned int ix = size_t(ii%nx);
    unsigned int iy = size_t( (ii/nx)%ny );
    unsigned int iz = size_t(ii/nxy);

    *theFileOut << (thePhantomMinusCorner.x() + (ix*2+1)*voxelHalfX)/CLHEP::cm << " " 
		<< (thePhantomMinusCorner.y() + (iy*2+1)*voxelHalfY)/CLHEP::cm << " " 
		<< (thePhantomMinusCorner.z() + (iz*2+1)*voxelHalfZ)/CLHEP::cm << " ";

    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    if( ite != scorerMap->end() ){
      *theFileOut << (*(ite->second))/theUnit / nev;
    } else {
      *theFileOut << 0.;
    }

    *theFileOut << G4endl;
    
  }

  CloseFileOut();
  
}
