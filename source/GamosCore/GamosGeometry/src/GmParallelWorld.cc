#include "GmParallelWorld.hh"

#include "GmGeometryUtils.hh"

GmParallelWorld::GmParallelWorld(G4String worldName, G4int index)
  : G4tgbParallelWorld(worldName, index)
{;}

GmParallelWorld::~GmParallelWorld()
{;}

#include "G4PhysicalVolumeStore.hh"

void GmParallelWorld::Construct()
{
  G4tgbParallelWorld::Construct();
    
  GmGeometryUtils::GetInstance()->RebuildDictionaries();
}
