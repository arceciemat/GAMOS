#include "G4LivermorePhotoElectricModel.hh"
#include "GmPhysicsGammaLowEner.hh"
#include "GmPhysicsVerbosity.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"

#include "G4PhotoElectricEffect.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4RegionStore.hh"

GmPhysicsGammaLowEner::GmPhysicsGammaLowEner(const G4String& name, G4int type): GmVPhysicsGamma(name,type)
{ }

GmPhysicsGammaLowEner::~GmPhysicsGammaLowEner()
{ }

void GmPhysicsGammaLowEner::ConstructProcess()
{
  // Add EPDL processes for gammas
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4double LivermoreHighEnergyLimit = CLHEP::GeV;
  G4cout << "GmPhysicsGammaLowEner::ConstructProcess " << G4endl;
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4ProcessManager* pmanager = particle -> GetProcessManager();
    G4String particleName = particle -> GetParticleName();
    
    if (particleName == "gamma") {

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* photModel = new G4LivermorePhotoElectricModel();

      photModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0, photModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4LivermoreComptonModel* theLivermoreComptonModel = 
	new G4LivermoreComptonModel();
      theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = 
	new G4LivermoreGammaConversionModel();
      theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      pmanager->AddDiscreteProcess(theRayleigh);
      
    }
  }
}
