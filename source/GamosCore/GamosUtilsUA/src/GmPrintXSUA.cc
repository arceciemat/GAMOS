#include "GmPrintXSUA.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"

//------------------------------------------------------------------
GmPrintXSUA::GmPrintXSUA()
{
}

//------------------------------------------------------------------
GmPrintXSUA::~GmPrintXSUA()
{
}

//------------------------------------------------------------------
void GmPrintXSUA::BeginOfRunAction( const G4Run* )
{
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( size_t ii = 0; ii < procVector->size(); ii++ ) {
      G4VProcess* proc = (*procVector)[ii];
      G4VEmProcess* procEM = dynamic_cast<G4VEmProcess*>(proc);
      if( procEM != 0 ) {
	G4cout << " GmPrintXSUA Process EM found " << procEM->GetProcessName() << " for " << particle->GetParticleName() << G4endl; // GDEB
	for( G4int im = 1; ; im++) {
	  G4VEmModel* modelEM = procEM->EmModel(im);
	  if( modelEM == 0 ) {
	    G4cout << im << " GmPrintXSUA Model EM NOT found " << G4endl; // GDEB
	    break;
	  } else {
	    G4cout << im << " GmPrintXSUA Model EM found " << modelEM->GetName() << " for " << procEM->GetProcessName() << " for " << particle->GetParticleName() << G4endl; // GDEB
	     std::vector<G4EmElementSelector*>* elemSelec = modelEM->GetElementSelectors();
	     if( elemSelec ) {
	       for( size_t ies = 0; ies < elemSelec->size(); ies++ ) {
		 (*elemSelec)[ies]->Dump();
	       }
	     }

	  }
	}
      }
    }

  }

}
