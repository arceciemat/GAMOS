#include "GmPhysicsCoulombScattering.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4eSingleCoulombScatteringModel.hh"
#include "G4IonCoulombScatteringModel.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


GmPhysicsCoulombScattering::GmPhysicsCoulombScattering()
{ }

GmPhysicsCoulombScattering::~GmPhysicsCoulombScattering()
{ }

void GmPhysicsCoulombScattering::ConstructParticle()
{
}

void GmPhysicsCoulombScattering::ConstructProcess()
{
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if( !pmanager ) {
      G4Exception("GmPhysicsCoulombScattering::ConstructProcess",
		  "Error",
		  FatalException,
		  G4String("No process found for particle " + particleName + ", you need to change your physics list ").c_str());
    } else {
      if ( particleName == "e-" || particleName == "e+" ) {
	G4CoulombScattering* cs = new G4CoulombScattering();
	G4eSingleCoulombScatteringModel* model = new G4eSingleCoulombScatteringModel();
	model->SetPolarAngleLimit(0.0);
	cs->AddEmModel(0, model);
	pmanager->AddDiscreteProcess(cs);
	pmanager->SetProcessOrderingToLast(cs,idxPostStep);
      } else if ( particleName == "proton" || particleName == "mu-" || particleName == "mu+" ) {
	G4CoulombScattering* cs = new G4CoulombScattering();
	G4eCoulombScatteringModel* model = new G4eCoulombScatteringModel();
	model->SetPolarAngleLimit(0.0);
	cs->AddEmModel(0, model);
	pmanager->AddDiscreteProcess(cs);
	pmanager->SetProcessOrderingToLast(cs,idxPostStep);
      } else if (particleName == "alpha" || particleName == "He3" || particleName == "GenericIon" ) {
	G4CoulombScattering* cs = new G4CoulombScattering();
	G4IonCoulombScatteringModel* model = new G4IonCoulombScatteringModel();
	model->SetPolarAngleLimit(0.0);
	cs->AddEmModel(0, model);
	pmanager->AddDiscreteProcess(cs);
	pmanager->SetProcessOrderingToLast(cs,idxPostStep);
      } else if ((!particle->IsShortLived()) &&
		 (particle->GetPDGCharge() != 0.0) && 
		 (particle->GetParticleName() != "chargedgeantino")) {
	//all others charged particles except geantino
	G4CoulombScattering* cs = new G4CoulombScattering();
	pmanager->AddDiscreteProcess(cs);   
	pmanager->SetProcessOrderingToLast(cs,idxPostStep);
      }
    }
  }
}

