#include "GmPhysicsPositronPenelope.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPhysicsMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4PenelopeIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

#include "G4eplusAnnihilation.hh"
#include "G4PenelopeAnnihilationModel.hh"

GmPhysicsPositronPenelope::GmPhysicsPositronPenelope(const G4String& name,G4int type): 
  G4VPhysicsConstructor(name,type)
{ }

GmPhysicsPositronPenelope::~GmPhysicsPositronPenelope()
{ }

void GmPhysicsPositronPenelope::ConstructProcess()
{
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();

  G4double PenelopeHighEnergyLimit = 1.0*CLHEP::GeV;
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "e+") 
    {
      GmPhysicsMultipleScattering* msc = new GmPhysicsMultipleScattering("msc","Positron");
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);

      //Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* theIoniPenelope = 
	new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eIoni->AddEmModel(0,theIoniPenelope,new G4UniversalFluctuation());
      eIoni->SetStepFunction(0.2, 100*CLHEP::um); //     
      pmanager->AddProcess(eIoni,                 -1, 2, 2);

       //Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* theBremPenelope = new 
	G4PenelopeBremsstrahlungModel();
      theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eBrem->AddEmModel(0,theBremPenelope);
      pmanager->AddProcess(eBrem, -1,-3, 3);
      
      //Annihilation
      G4eplusAnnihilation* eAnni = new G4eplusAnnihilation();
      G4PenelopeAnnihilationModel* theAnnPenelope = new 
	G4PenelopeAnnihilationModel();
      theAnnPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eAnni->AddEmModel(0,theAnnPenelope);
      pmanager->AddProcess(eAnni,0,-1, 4);
    }   
  }
}
