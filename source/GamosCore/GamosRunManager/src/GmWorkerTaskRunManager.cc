#include "GmWorkerTaskRunManager.hh"

#ifdef G4MULTITHREADED
#include "G4TaskRunManager.hh"

//----------------------------------------------------------------------
void GmWorkerTaskRunManager::ProcessUI()
{
  G4TaskRunManager* mrm = G4TaskRunManager::GetMasterRunManager();
  if( mrm == nullptr ) return;
  //--- the commands have already been applied in this thread by G4TaskRunManager (see header)
  processedCommandStack = mrm->GetCommandStack();
}
#endif
