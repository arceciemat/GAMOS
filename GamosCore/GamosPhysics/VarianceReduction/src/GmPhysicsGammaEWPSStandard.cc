#include "GmPhysicsGammaEWPSStandard.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GmPSComptonScattering.hh"
#include "GmEWPSKleinNishinaComptonModel.hh"
#include "GmPSPhotoElectricEffect.hh"
#include "GmEWPSPEEffectModel.hh"
#include "GmPSGammaConversion.hh"
#include "GmEWPSBetheHeitlerModel.hh"

GmPhysicsGammaEWPSStandard::GmPhysicsGammaEWPSStandard(const G4String& name): GmVPhysicsGamma(name)
{ }

GmPhysicsGammaEWPSStandard::~GmPhysicsGammaEWPSStandard()
{ }

void GmPhysicsGammaEWPSStandard::ConstructProcess()
{
  // Add standard processes for gammas
  
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();

      if (particleName == "gamma") 
	{
	  GmPSPhotoElectricEffect* photoElectric = new GmPSPhotoElectricEffect("GmEWPSphot"); 
	  GmEWPSPEEffectModel* photoElectricModel = new GmEWPSPEEffectModel(G4Gamma::Gamma(),"GmEWPSphotModel");
	  photoElectric->AddEmModel(0,photoElectricModel);
	  photoElectric->AddPSEmModel(photoElectricModel);
	  manager -> AddDiscreteProcess( photoElectric );

	  GmPSComptonScattering* compt = new GmPSComptonScattering("GmEWPScompt");
	  GmEWPSKleinNishinaComptonModel* theComptonModel = 
	    new GmEWPSKleinNishinaComptonModel(G4Gamma::Gamma(),"GmEWPScomptModel");
	  theComptonModel->SetHighEnergyLimit(100.*TeV);
	  compt->AddEmModel(0, theComptonModel);
	  compt->AddPSEmModel(theComptonModel);
	  manager->AddDiscreteProcess(compt);

	  GmPSGammaConversion* conv = new GmPSGammaConversion("GmEWPSconv");
	  GmEWPSBetheHeitlerModel* theGammaConvModel = new GmEWPSBetheHeitlerModel(G4Gamma::Gamma(),"GmEWPScomptModel");
	  conv->AddEmModel(0, theGammaConvModel);
	  conv->AddPSEmModel(theGammaConvModel);
	  manager -> AddDiscreteProcess(conv);
	}   
    }
}
