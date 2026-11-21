#include "GmActionInitialization.hh"
#include "GmRunManager.hh"

//----------------------------------------------------------------------
void GmActionInitialization::BuildForMaster() const
{
  // Master user actions are created by the UI commands themselves
  // (/gamos/userAction ...), as in sequential mode
}

//----------------------------------------------------------------------
void GmActionInitialization::Build() const
{
  // Called once per worker thread, before the worker replays the master commands
  GmRunManager::GetRunManager()->BuildForWorker();
}
