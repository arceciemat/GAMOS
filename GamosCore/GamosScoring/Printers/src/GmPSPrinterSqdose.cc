#include "GmPSPrinterSqdose.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

GmPSPrinterSqdose::GmPSPrinterSqdose(G4String name) : GmVPSPrinter( name )
{
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");
  theOutputType = "sqdose";
}

void GmPSPrinterSqdose::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 0 && params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterSqdose::SetParameters",
		"There should be two optional parameters: UNIT UNIT_NAME",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ){
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
    //    G4cout << this << " GmPSPrinterSqdose::SetParameters " << params[0] << " " << theUnit << " " << theUnitName << G4endl;
  }
}

void GmPSPrinterSqdose::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  if( !theScorer->ScoreErrors() ) {
    G4Exception("GmPSPrinterSqdose::DumpAll ",
		"Scorer is not scoring errors",FatalErrorInArgument,theScorer->GetName());
  }

  WriteSqdose(RunMap,theScorer);
}


//-----------------------------------------------------------------------
GmSqdoseHeader* GmPSPrinterSqdose::BuildSqdoseHeader()
{
  GmSqdoseHeader* header = new GmSqdoseHeader;

  float nev = GmNumberOfEvent::GetNumberOfEvent();
  header->SetNumberOfEvents( nev );

  thePhantomParam = GmRegularParamUtils::GetInstance()->GetPhantomParam( TRUE );
  unsigned int nx = thePhantomParam->GetNoVoxelX();
  unsigned int ny = thePhantomParam->GetNoVoxelY();
  unsigned int nz = thePhantomParam->GetNoVoxelZ();
  header->SetNoVoxelX( nx );
  header->SetNoVoxelY( ny );
  header->SetNoVoxelZ( nz );

  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner = (thePhantomParam->GetTranslation(0) + thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxel()-1) )/ 2;
  float voxelX = thePhantomParam->GetVoxelHalfX()*2;
  float voxelY = thePhantomParam->GetVoxelHalfY()*2;
  float voxelZ = thePhantomParam->GetVoxelHalfZ()*2;
  //  G4cout << " phantom minus corner (temp) " << thePhantomMinusCorner << " translation 0 " << thePhantomParam->GetTranslation(0) << " translation 1 " << thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxel()-1) << G4endl;
  // G4cout << " voxelX " << thePhantomParam->GetVoxelHalfX() << " = " << voxelX <<  "  " << -voxelX*nx/2. << " " << G4ThreeVector( -nx*voxelX/2.,-ny*voxelY/2.,-nz*voxelZ/2.) << G4endl;
  thePhantomMinusCorner -= G4ThreeVector(voxelX*nx/2.,voxelY*ny/2.,voxelZ*nz/2.);
  // G4cout << " phantom minus corner " << thePhantomMinusCorner << G4endl;
  //----- Add coordinates of phantom volume 
  G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
  GmTouchable* pcont = touchs[0];

  thePhantomMinusCorner += pcont->GetGlobalPosition();
  // G4cout << " phantom minus corner " << thePhantomMinusCorner << *pcont << G4endl;
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    delete *ite;
  }

  std::vector<float> vval;
  for( unsigned int ii = 0; ii <= nx; ii++ ) {
    vval.push_back( thePhantomMinusCorner.x() + ii*voxelX );
    //    G4cout << ii << " set voxellimitX " << thePhantomMinusCorner.x() + ii*voxelX << G4endl;
  }

  header->SetVoxelLimitsX( vval );

  vval.clear();
  //  *theDoseFile << G4endl;
  for( unsigned int ii = 0; ii <= ny; ii++ ) {
    vval.push_back( thePhantomMinusCorner.y() + ii*voxelY );
    // *theDoseFile << thePhantomMinusCorner.y() + ii*voxelY << " ";
  }
  //  *theDoseFile << G4endl;
  header->SetVoxelLimitsY( vval );

  vval.clear();
  for( unsigned int ii = 0; ii <= nz; ii++ ) {
    vval.push_back( thePhantomMinusCorner.z() + ii*voxelZ );
    //  *theDoseFile << thePhantomMinusCorner.z() + ii*voxelZ << " ";
  }
  header->SetVoxelLimitsZ( vval );

  return header;
}

//-----------------------------------------------------------------------
void GmPSPrinterSqdose::WriteSqdose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  G4String fileName = "sqdose.out";
  G4String scorerName = "";
  if( theScorer ) scorerName = theScorer->GetName();
  fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+"_"+scorerName+":FileName",fileName);

  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if(suffix != "" ) fileName += suffix;

  G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  if(prefix != "" ) fileName = prefix + fileName;

  OpenFileOut(fileName);

  GmSqdoseHeader* header = BuildSqdoseHeader();

  G4int nvoxels = thePhantomParam->GetNoVoxelX() * thePhantomParam->GetNoVoxelY() * thePhantomParam->GetNoVoxelZ();
  //  G4int nvoxZ = thePhantomParam->GetNoVoxelZ();
  
  //----- Loop to all voxels and print dose
  std::vector<float> doses;
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    float dose;
    if( ite != scorerMap->end() ){
      dose = (*(ite->second))/theUnit;
    } else {
      dose = 0.;
    }
    doses.push_back( dose );
    //    G4cout << ii << " GmPSPrinterSqdose::WriteSqdose dose " << dose << G4endl;
  }
  
  //----- Loop to all voxels and print dose error
  std::vector<float> doseSqs;
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    float dose2;
    if( ite != scorerMap->end() ){
      dose2 = theScorer->GetSumV2(ite->first)/(theUnit*theUnit);
    } else {
      dose2 = 0.;
    }
    doseSqs.push_back( dose2 );
    //    G4cout << ii << " GmPSPrinterSqdose::WriteSqdose dose " << dose2 << G4endl;
  }


  GmSqdose sqdose;
  sqdose.SetHeader( header );
  sqdose.SetDoses( doses );
  sqdose.SetDoseSqs( doseSqs );

  sqdose.Print( theFileOut );

  CloseFileOut();
}
