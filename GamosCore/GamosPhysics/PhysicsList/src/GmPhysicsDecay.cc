#include "GmPhysicsDecay.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsDecay::GmPhysicsDecay()
{ }

GmPhysicsDecay::~GmPhysicsDecay()
{ }

void GmPhysicsDecay::ConstructProcess()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();
  while( (*theParticleIterator)() ) {
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4ProcessManager* pmanager = particle -> GetProcessManager();
    if( GmG4Utils::CheckProcessExists( pmanager, "Decay" )) break;
    if (theDecayProcess -> IsApplicable(*particle) && !particle->IsShortLived()) { 
      pmanager -> AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager -> SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager -> SetProcessOrdering(theDecayProcess, idxAtRest);
    }
    
  }
}
