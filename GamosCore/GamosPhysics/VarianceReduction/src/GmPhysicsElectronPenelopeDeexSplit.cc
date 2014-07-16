#include "GmPhysicsElectronPenelopeDeexSplit.hh"
#include "G4eBremsstrahlung.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

//??#include "GmPSeIonisation.hh"
#include "GmDeexSpliteIonisation.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"


GmPhysicsElectronPenelopeDeexSplit::GmPhysicsElectronPenelopeDeexSplit(const G4String& name): GmVPhysicsElectron(name)
{ }

GmPhysicsElectronPenelopeDeexSplit::~GmPhysicsElectronPenelopeDeexSplit()
{ }

void GmPhysicsElectronPenelopeDeexSplit::ConstructProcess()
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
      //      GmPSeIonisation* eIoni = new GmPSeIonisation();
      GmDeexSpliteIonisation* eIoni = new GmDeexSpliteIonisation();

      G4PenelopeIonisationModel* ioniModel = new G4PenelopeIonisationModel();

      AddDeexcitation();

      eIoni->AddEmModel(0, ioniModel, new G4UniversalFluctuation() );
      //??      eIoni->AddPSEmModel( ioniModel ),
      eIoni->SetStepFunction(0.2, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 2, 2);

      // Bremsstrahlung
      G4eBremsstrahlung* eBremProcess = new G4eBremsstrahlung("brems");
      G4PenelopeBremsstrahlungModel* bremsModel = new G4PenelopeBremsstrahlungModel();
      // Penelope has no angular distribution choice      SelectBremssAngularDist( bremsModel );
      eBremProcess->AddEmModel(0, bremsModel);
      pmanager->AddProcess(eBremProcess, -1,-3, 3);


    }
  }
}
