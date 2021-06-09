#include "GmGeomTextDetectorBuilder.hh"
#include "GmGeomTextLineProcessor.hh"
#include "GmOpticalPropertiesMgr.hh"
#include "GmRegionCutsMgr.hh"

#include "G4tgrFileReader.hh"
#include "G4tgrVolumeMgr.hh"

//---------------------------------------------------------------------
GmGeomTextDetectorBuilder::GmGeomTextDetectorBuilder() : G4tgbDetectorBuilder()
{
}


//---------------------------------------------------------------------
const G4tgrVolume* GmGeomTextDetectorBuilder::ReadDetector()
{
  //------------------- construct g4 geometry
  GmGeomTextLineProcessor* tlproc = new GmGeomTextLineProcessor;
  G4tgrFileReader* tfr = G4tgrFileReader::GetInstance();
  tfr->SetLineProcessor( tlproc );
  tfr->ReadFiles();
  //---------- find top G4tgrVolume 
  G4tgrVolumeMgr* tgrVolmgr = G4tgrVolumeMgr::GetInstance();
  const G4tgrVolume* tgrVoltop = tgrVolmgr->GetTopVolume();  
  return tgrVoltop;
}


//---------------------------------------------------------------------
G4VPhysicalVolume* GmGeomTextDetectorBuilder::ConstructDetectorGAMOS( const G4tgrVolume* tgrVoltop, G4int , G4bool bBuildRegionCuts )
{
  G4VPhysicalVolume* topPV = G4tgbDetectorBuilder::ConstructDetector( tgrVoltop );

  if( bBuildRegionCuts ) {
    //--- Create regions
    GmRegionCutsMgr::GetInstance()->BuildRegions();
    
    //--- Set cuts to regions
    GmRegionCutsMgr::GetInstance()->BuildProductionCuts();
  }

  GmOpticalPropertiesMgr::GetInstance()->BuildG4();

  return topPV;
}
