#include "GmPhysicsElectronUBSLowEner.hh"
#include "G4eBremsstrahlung.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "GmPSeBremsstrahlung.hh"
#include "GmBSLivermoreBremsstrahlungModel.hh"
#include "GmPSZPlaneDirCheckerAcceptAll.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


GmPhysicsElectronUBSLowEner::GmPhysicsElectronUBSLowEner(const G4String& name): GmVPhysicsElectron(name)
{ }

GmPhysicsElectronUBSLowEner::~GmPhysicsElectronUBSLowEner()
{ }

void GmPhysicsElectronUBSLowEner::ConstructProcess()
{
  // Add EEDL processes for electrons
  
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "e-") 
    {
      GmPhysicsMultipleScattering* msc = new GmPhysicsMultipleScattering("msc","Electron");
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,                   -1, 1, 1);
      
      // Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4LivermoreIonisationModel* ioniModel = new G4LivermoreIonisationModel();
      eIoni->AddEmModel(0, ioniModel, new G4UniversalFluctuation() );

      //      AddDeexcitation( eIoni, ioniModel );

      eIoni->SetStepFunction(0.2, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 2, 2);

      // Bremsstrahlung
      GmPSeBremsstrahlung* eBremProcess = new GmPSeBremsstrahlung("GmUBSBrems");
      //      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung(); // this makes it crash
      //  eBremProcess->SetWrappedProcess(eBrem);
      GmBSLivermoreBremsstrahlungModel* bremsModel = new GmBSLivermoreBremsstrahlungModel();
      SelectBremssAngularDist( bremsModel );
      bremsModel->SetPlaneDirChecker( new GmPSZPlaneDirCheckerAcceptAll() );
      eBremProcess->AddEmModel(0, bremsModel);
      eBremProcess->AddPSEmModel(bremsModel);
    
      pmanager->AddProcess(eBremProcess, -1,-3, 3);

    }
  }
}
