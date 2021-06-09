#include "GmPhysicsGammaPenelopeDeexSplit.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "GmPSPhotoElectricEffect.hh"
#include "GmDeexSplitPenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "GmDeexSplitPenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

#include "G4RegionStore.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmPhysicsGammaPenelopeDeexSplit::GmPhysicsGammaPenelopeDeexSplit(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaPenelopeDeexSplit::~GmPhysicsGammaPenelopeDeexSplit()
{ }

void GmPhysicsGammaPenelopeDeexSplit::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  theParticleIterator -> reset();

  G4double PenelopeHighEnergyLimit = GeV;
  while( (*theParticleIterator)() )
  {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
    if (particleName == "gamma") {

      GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect();
      GmDeexSplitPenelopePhotoElectricModel* photoElectricModel = new GmDeexSplitPenelopePhotoElectricModel();
      AddDeexcitation();
      photoElectricModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      photoElectric->AddEmModel(0, photoElectricModel);
      photoElectric->AddPSEmModel(photoElectricModel);
      pmanager->AddDiscreteProcess(photoElectric);

      G4ComptonScattering* compt = new G4ComptonScattering();
      GmDeexSplitPenelopeComptonModel* comptModel = 
	new GmDeexSplitPenelopeComptonModel();
      comptModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      compt->AddEmModel(0, comptModel);
      pmanager->AddDiscreteProcess(compt);

      G4GammaConversion* conv = new G4GammaConversion();
      G4PenelopeGammaConversionModel* convModel = 
	new G4PenelopeGammaConversionModel();
      convModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      conv->AddEmModel(0, convModel);
      pmanager->AddDiscreteProcess(conv);

      G4RayleighScattering* rayl = new G4RayleighScattering();
      G4PenelopeRayleighModel* raylModel = new G4PenelopeRayleighModel();
      raylModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      rayl->AddEmModel(0, raylModel);
      pmanager->AddDiscreteProcess(rayl);

    }
  }
}
