#ifndef GmWorkerTaskRunManager_H
#define GmWorkerTaskRunManager_H

// GAMOS multithreading with G4TaskRunManager (Geant4 default run manager type).
// G4TaskRunManager applies the master UI commands to each worker thread when the workers are initialised
// (InitializeWorker) and at each BeamOn (CreateAndStartWorkers), and then G4WorkerTaskRunManager::ProcessUI()
// applies the same command stack a second time. Geant4 commands are usually idempotent, but GAMOS commands create
// objects (generators, user actions, scorers, ...), so they must be applied only once.
// ProcessUI() is overridden to mark the command stack as processed without applying it again.
// (Valid with the PTL thread pool, where all worker threads exist when the commands are distributed.)

// G4MULTITHREADED comes from G4GlobalConfig.hh (CMake build) or from the compiler flags (GNUmake build)
#include "G4Types.hh"
#ifdef G4MULTITHREADED
#include "G4WorkerTaskRunManager.hh"
#include "G4UserTaskThreadInitialization.hh"

class GmWorkerTaskRunManager : public G4WorkerTaskRunManager
{
public:
  GmWorkerTaskRunManager() : G4WorkerTaskRunManager() {}
  virtual ~GmWorkerTaskRunManager() {}
  virtual void ProcessUI() override;
};

class GmUserTaskThreadInitialization : public G4UserTaskThreadInitialization
{
public:
  GmUserTaskThreadInitialization() {}
  virtual ~GmUserTaskThreadInitialization() {}
  virtual G4WorkerRunManager* CreateWorkerRunManager() const override {
    return new GmWorkerTaskRunManager();
  }
};
#endif

#endif
