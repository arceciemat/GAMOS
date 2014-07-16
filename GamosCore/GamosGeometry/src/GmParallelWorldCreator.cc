// -------------------------------------------------------------------------

#include "GmParallelWorldCreator.hh"
#include "GmParallelWorld.hh"

//--------------------------------------------------------------------
G4VUserParallelWorld* GmParallelWorldCreator::CreateParallelWorld( const G4String& parallelWorldName, const G4int index )
{
  return new GmParallelWorld( parallelWorldName, index );
}
