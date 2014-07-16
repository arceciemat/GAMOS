#include "GmPhysicsPositronUBSStandard.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "GmPSeBremsstrahlung.hh"
#include "GmPSeplusAnnihilation.hh"
#include "GmEWPSBremsstrahlungModel.hh"
#include "G4eplusAnnihilation.hh"
#include "GmPSZPlaneDirCheckerAcceptAll.hh"

GmPhysicsPositronUBSStandard::GmPhysicsPositronUBSStandard(const G4String& name): G4VPhysicsConstructor(name)
{ }

GmPhysicsPositronUBSStandard::~GmPhysicsPositronUBSStandard()
{ }

void GmPhysicsPositronUBSStandard::ConstructProcess()
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
	  manager -> AddProcess(new G4eIonisation,        -1, 2,2);
	  // Bremsstrahlung
	  GmPSeBremsstrahlung* eBremProcess = new GmPSeBremsstrahlung("GmUBSBrems");
	  GmEWPSBremsstrahlungModel* bremsModel = new GmEWPSBremsstrahlungModel();
	  bremsModel->SetPlaneDirChecker( new GmPSZPlaneDirCheckerAcceptAll() );
	  eBremProcess->AddEmModel(0, bremsModel);
	  eBremProcess->AddPSEmModel(bremsModel);
	  manager->AddProcess(eBremProcess, -1,-3, 3);
	  
	  // Annihilation
	  manager -> AddProcess(new G4eplusAnnihilation,   0,-1,4); 
	  
	}   
    }
}
