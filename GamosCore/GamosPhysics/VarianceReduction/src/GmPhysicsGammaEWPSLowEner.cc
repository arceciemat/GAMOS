#include "GmPhysicsGammaEWPSLowEner.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPSPhotoElectricEffect.hh"
#include "GmEWPSLivermorePhotoElectricModel.hh"

#include "GmPSComptonScattering.hh"
#include "GmEWPSLivermoreComptonModel.hh"

#include "GmPSGammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

GmPhysicsGammaEWPSLowEner::GmPhysicsGammaEWPSLowEner(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaEWPSLowEner::~GmPhysicsGammaEWPSLowEner()
{ }

void GmPhysicsGammaEWPSLowEner::ConstructProcess()
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
      GmEWPSLivermorePhotoElectricModel* photoElectricModel = 
	new GmEWPSLivermorePhotoElectricModel();
      //      AddDeexcitation( photoElectric, photoElectricModel);
      photoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      photoElectric->AddEmModel(0, photoElectricModel);
      photoElectric->AddPSEmModel(photoElectricModel);
      pmanager->AddDiscreteProcess(photoElectric);

      GmPSComptonScattering* compt = new GmPSComptonScattering();
      GmEWPSLivermoreComptonModel* comptModel = 
	new GmEWPSLivermoreComptonModel();
      comptModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);
      compt->AddPSEmModel(comptModel);

      GmPSGammaConversion* conv = new GmPSGammaConversion();
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
