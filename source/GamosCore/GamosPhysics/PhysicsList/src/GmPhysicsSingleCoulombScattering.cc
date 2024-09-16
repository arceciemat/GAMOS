#include "GmPhysicsSingleCoulombScattering.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4hCoulombScatteringModel.hh"
#include "G4IonCoulombScatteringModel.hh"
#include "G4eSingleCoulombScatteringModel.hh"
#include "G4IonCoulombScatteringModel.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


GmPhysicsSingleCoulombScattering::GmPhysicsSingleCoulombScattering()
{ }

GmPhysicsSingleCoulombScattering::~GmPhysicsSingleCoulombScattering()
{ }

void GmPhysicsSingleCoulombScattering::ConstructParticle()
{
}

void GmPhysicsSingleCoulombScattering::ConstructProcess()
{
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if( !pmanager ) {
      G4Exception("GmPhysicsSingleCoulombScattering::ConstructProcess",
		  "Error",
		  FatalException,
		  G4String("No process found for particle " + particleName + ", you need to change your physics list ").c_str());
    } else {
      G4CoulombScattering* cs = new G4CoulombScattering();
      G4eSingleCoulombScatteringModel* model = new G4eSingleCoulombScatteringModel();
      //model->SetLowEnergyThreshold(10*eV);                                                                      
      model->SetPolarAngleLimit(0.0);
      cs->AddEmModel(0, model);
      pmanager->AddDiscreteProcess(cs);

    }
  }
}


