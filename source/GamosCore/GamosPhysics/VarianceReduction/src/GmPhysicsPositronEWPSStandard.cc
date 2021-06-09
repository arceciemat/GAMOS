#include "GmPhysicsPositronEWPSStandard.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
//??#include "GmPSeIonisation.hh"
#include "G4eIonisation.hh"
#include "GmPSeBremsstrahlung.hh"
#include "GmPSeplusAnnihilation.hh"
#include "GmEWPSBremsstrahlungModel.hh"
#include "GmEWPSeeToTwoGammaModel.hh"

GmPhysicsPositronEWPSStandard::GmPhysicsPositronEWPSStandard(const G4String& name): G4VPhysicsConstructor(name)
{ }

GmPhysicsPositronEWPSStandard::~GmPhysicsPositronEWPSStandard()
{ }

void GmPhysicsPositronEWPSStandard::ConstructProcess()
{
  // Add standard processes for positrons
  
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
     
      if (particleName == "e+") 
	{
	  GmPhysicsMultipleScattering* msc = new GmPhysicsMultipleScattering("msc","Positron");
	  manager -> AddProcess(msc, -1, 1,1);
	  //	  manager -> AddProcess(new GmPSeIonisation,        -1, 2,2);
	  manager -> AddProcess(new G4eIonisation,        -1, 2,2);
	  // Bremsstrahlung
	  GmPSeBremsstrahlung* eBremProcess = new GmPSeBremsstrahlung("GmEWPSBrems");
	  GmEWPSBremsstrahlungModel* bremsModel = new GmEWPSBremsstrahlungModel();
	  eBremProcess->AddEmModel(0, bremsModel);
	  eBremProcess->AddPSEmModel(bremsModel);
	  manager->AddProcess(eBremProcess, -1,-3, 3);
	  
	  // Annihilation
	  GmPSeplusAnnihilation* annihProcess = new GmPSeplusAnnihilation("GmEWPSAnnihil");
	  GmEWPSeeToTwoGammaModel* annihModel = new GmEWPSeeToTwoGammaModel();
	  annihProcess->AddEmModel(0, annihModel);
	  annihProcess->AddPSEmModel(annihModel);
	  manager->AddProcess(annihProcess, -1,-4, 4);
	  
	}   
    }
}
