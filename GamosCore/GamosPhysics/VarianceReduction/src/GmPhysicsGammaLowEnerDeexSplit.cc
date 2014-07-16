#include "GmPhysicsGammaLowEnerDeexSplit.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPSPhotoElectricEffect.hh"
#include "GmDeexSplitLivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "GmDeexSplitLivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

#include "G4RegionStore.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmPhysicsGammaLowEnerDeexSplit::GmPhysicsGammaLowEnerDeexSplit(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaLowEnerDeexSplit::~GmPhysicsGammaLowEnerDeexSplit()
{ }

void GmPhysicsGammaLowEnerDeexSplit::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  theParticleIterator -> reset();

  G4double LivermoreHighEnergyLimit = GeV;
  while( (*theParticleIterator)() )
  {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
    if (particleName == "gamma") {

      GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect();
      GmDeexSplitLivermorePhotoElectricModel* photoElectricModel = new GmDeexSplitLivermorePhotoElectricModel();
      AddDeexcitation();
      photoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      photoElectric->AddEmModel(0, photoElectricModel);
      photoElectric->AddPSEmModel(photoElectricModel);
      pmanager->AddDiscreteProcess(photoElectric);

      G4ComptonScattering* compt = new G4ComptonScattering();
      GmDeexSplitLivermoreComptonModel* comptModel = 
	new GmDeexSplitLivermoreComptonModel();
      comptModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);

      G4GammaConversion* conv = new G4GammaConversion();
      G4LivermoreGammaConversionModel* convModel = 
	new G4LivermoreGammaConversionModel();
      convModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4LivermoreRayleighModel* raylModel = new G4LivermoreRayleighModel();
      raylModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);

    }
  }
}
