// ---------------------------------------------------------------------------
#include "globals.hh"
#include "GmPhysicsParallel.hh"

#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4VUserParallelWorld.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "G4UIcommand.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

// ---------------------------------------------------------------------------
GmPhysicsParallel::GmPhysicsParallel()
{
}

// ---------------------------------------------------------------------------
GmPhysicsParallel::~GmPhysicsParallel()
{
}

// ---------------------------------------------------------------------------
void GmPhysicsParallel::ConstructProcess()
{
  std::vector<G4VUserParallelWorld*> parallelWorlds = G4tgbParallelGeomMgr::GetInstance()->GetParallelWorlds();

  for( size_t ii = 0; ii < parallelWorlds.size(); ii++ ) {
    G4String paralWorldName = parallelWorlds[ii]->GetName();
    G4ParallelWorldProcess* theParallelWorldProcess
      = new G4ParallelWorldProcess("ParaWorldProc_"+paralWorldName);
    theParallelWorldProcess->SetParallelWorld(paralWorldName);
    if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsParallel:LayeredMaterial",1)) ) {
      theParallelWorldProcess->SetLayeredMaterialFlag();
    }
    
    G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      pmanager->AddProcess(theParallelWorldProcess);
      //    pmanager->SetProcessOrderingToLast(theParallelWorldProcess, idxAtRest);
      //    pmanager->SetProcessOrdering(theParallelWorldProcess, idxAlongStep, 1);
      //   pmanager->SetProcessOrderingToLast(theParallelWorldProcess, idxPostStep);
      pmanager->SetProcessOrdering(theParallelWorldProcess, idxAtRest, 9999);
      pmanager->SetProcessOrdering(theParallelWorldProcess, idxAlongStep, 1);
      pmanager->SetProcessOrdering(theParallelWorldProcess, idxPostStep, 9999);
    }
  }
  /*  std::vector<G4VUserParallelWorld*> parallelWorlds = G4tgbParallelGeomMgr::GetInstance()->GetParallelWorlds();
  for( unsigned int ii = 0; ii < parallelWorlds.size(); ii++ ) {
    theParallelWorldProcess->SetParallelWorld( parallelWorlds[ii]->GetName() );
  }
  */
  
}
