#include "GmGeometryUseMaterials.hh"
#include <vector>
#include <map>
#include "GmGeomUseMateDetectorBuilder.hh"
#include "GmGeometryUtils.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "GmParallelWorldCreator.hh"
#include "G4tgrMessenger.hh"
#include "G4VUserParallelWorld.hh"

GmGeometryUseMaterials::GmGeometryUseMaterials()
{
  new G4tgrMessenger;
}


//---------------------------------------------------------------------
G4VPhysicalVolume* GmGeometryUseMaterials::Construct()
{

  //------------------------------------------------ 
  // Read the text files 
  //------------------------------------------------ 
  GmGeomUseMateDetectorBuilder* gtb = new GmGeomUseMateDetectorBuilder;
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->SetDetectorBuilder( gtb );

  const G4tgrVolume* tgrVoltop = gtb->ReadDetector();
  //------------------------------------------------ 
  // Build the Geant4 mass geometry
  //------------------------------------------------ 

  G4VPhysicalVolume* physiWorld = gtb->ConstructDetector(tgrVoltop, -1 );

  return physiWorld;
}
