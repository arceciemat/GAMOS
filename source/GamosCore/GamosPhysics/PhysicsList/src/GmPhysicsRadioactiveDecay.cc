#include "GmPhysicsRadioactiveDecay.hh"
#include "G4ProcessManager.hh"

#include "G4RadioactiveDecay.hh"
#include "G4GenericIon.hh"
#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::GmPhysicsRadioactiveDecay()
  : theRadioactiveDecay(0)
{}

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::~GmPhysicsRadioactiveDecay()
{
  delete theRadioactiveDecay;
}
 
//----------------------------------------------------------------------
void GmPhysicsRadioactiveDecay::ConstructProcess()
{
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  if( !theRadioactiveDecay ) theRadioactiveDecay = new G4RadioactiveDecay();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if(particle->GetParticleName() != "GenericIon" && 
       ( particle->GetParticleType() != "nucleus" || particle->GetPDGLifeTime() < 0. )) continue;
    // THIS HAS A BUG IN GEANT4   if( ! theRadioactiveDecay->IsApplicable( *particle ) ) continue;

    G4ProcessManager* pmanager = particle->GetProcessManager();

    if( GmG4Utils::CheckProcessExists( pmanager, "RadioactiveDecay" )) break;
    //  if not Found
    //      G4cout << " GmPhysicsRadioactiveDecay::ConstructProcess  "<< particle->GetParticleName()  << G4endl;
    //  G4GenericIon* ion = G4GenericIon::GenericIon();
    //  G4ProcessManager* pmanager = ion->GetProcessManager();
    pmanager->AddProcess(theRadioactiveDecay);
    pmanager->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
    pmanager->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
  }
}


