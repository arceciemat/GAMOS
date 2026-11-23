#include "GmPhysicsPositronStandard.hh"
#include "GmAcollinearEplusAnnihilation.hh" //gdl

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GmPhysicsMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4eplusAnnihilation.hh"
#include "G4Generator2BS.hh"

GmPhysicsPositronStandard::GmPhysicsPositronStandard(const G4String& name,G4int type): GmVPhysicsElectron(name,type)
{ }

GmPhysicsPositronStandard::~GmPhysicsPositronStandard()
{ }

void GmPhysicsPositronStandard::ConstructProcess()
{
  // Add standard processes for positrons  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
     
      if (particleName == "e+") 
	{
	  manager -> AddProcess(new GmPhysicsMultipleScattering("msc","Positron"), -1, 1,1);
	  manager -> AddProcess(new G4eIonisation,        -1, 2,2);
	  //-	  manager -> AddProcess(new G4eBremsstrahlung,    -1, 3,3);
	  G4SeltzerBergerModel* model = new G4SeltzerBergerModel();
	  G4eBremsstrahlungRelModel* model2 = new G4eBremsstrahlungRelModel();
	  G4eBremsstrahlung* brems = new G4eBremsstrahlung();
	  model->SetLowEnergyLimit(brems->MinKinEnergy());
	  //	  model->SetHighEnergyLimit(model2->LowEnergyLimit());
	  model->SetHighEnergyLimit(1.*CLHEP::GeV);
	  model2->SetHighEnergyLimit(brems->MaxKinEnergy());                
	  /*	  G4VEmFluctuationModel* fm = 0;
	  brems->AddEmModel(0, model, fm);
	  brems->AddEmModel(1, model2, fm); */
	  brems->AddEmModel(0, model);
	  brems->AddEmModel(1, model2); 
	  SelectBremssAngularDist( model, "Positron" );
	  manager->AddProcess(brems,  -1, 3, 3); 


	  G4bool bAcollinear = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsPositronStandard:ePlusAcollinearAnnihilation",1));
	  if( !bAcollinear ) {
	    manager -> AddProcess(new G4eplusAnnihilation,   0,-1,4); 
	  } else {
	    manager -> AddProcess(new GmAcollinearEplusAnnihilation,   0,-1,4); //gdl
	  }

	}   
    }
}
