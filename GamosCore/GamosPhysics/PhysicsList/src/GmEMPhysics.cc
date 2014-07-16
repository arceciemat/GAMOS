#include "GmEMPhysics.hh"
#include "GmPhysicsMessenger.hh"
#include "GmPhysicsGammaStandard.hh"
#include "GmPhysicsGammaLowEner.hh"
#include "GmPhysicsGammaPenelope.hh"
#include "GmPhysicsElectronStandard.hh"
#include "GmPhysicsElectronLowEner.hh"
#include "GmPhysicsElectronPenelope.hh"
#include "GmPhysicsPositronStandard.hh"
#include "GmPhysicsPositronPenelope.hh"
#include "GmPhysicsDecay.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4BosonConstructor.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4EmProcessOptions.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


GmEMPhysics::GmEMPhysics(): G4VModularPhysicsList(),
					      electronIsRegistered(false), 
					      positronIsRegistered(false), 
					      gammaIsRegistered(false)
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 0.1 * CLHEP::mm;
  messenger = new GmPhysicsMessenger(this);
  SetVerboseLevel(1);
  ConstructParticles();

}

GmEMPhysics::~GmEMPhysics()
{ 
  //  delete messenger;
}

#include "G4ParticleTable.hh"

void GmEMPhysics::ConstructParticles()
{
  G4BosonConstructor bosonC;
  bosonC.ConstructParticle();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
  }

}

G4bool GmEMPhysics::ReplacePhysicsList(const G4String& name)
{
  G4cout << "GmEMPhysics::Replaceing PhysicsList component " << name << G4endl;
  
  //
  // Electromagnetic GamosEMLowEnPhysics. 
  //
  // The user can choose three alternative approaches:
  // Standard, Low Energy based on the Livermore libraries and Low Energy Penelope
  //
  G4bool bFound = true;

  // Register standard processes for gammas
  if (name == "gamma-standard") 
    {
      if (gammaIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- gamma List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsGammaStandard(name) );
	  gammaIsRegistered = true;
	}
  
      // Register LowE-EPDL processes for gammas
    } else if (name == "gamma-lowener"  
    || name == "gamma-epdl") 
    {
      if (gammaIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- gamma List already existing"
                 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsGammaLowEner(name) );
	  gammaIsRegistered = true;
	}

  // Register processes a' la Penelope for gammas
    } else if (name == "gamma-penelope")
    {
      if (gammaIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- gamma List already existing" 
                 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsGammaPenelope(name) );
	  gammaIsRegistered = true;
	}
  
      // Register standard processes for electrons
    } else if (name == "electron-standard") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing" 
		 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsElectronStandard(name) );	  
	  electronIsRegistered = true;
	}
      
      // Register LowE-EEDL processes for electrons
    } else if (name == "electron-lowener"  
      || name == "electron-eedl") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsElectronLowEner(name) );
	  electronIsRegistered = true;
	}
      
      // Register processes a' la Penelope for electrons
    } else if (name == "electron-penelope")
    {
      if (electronIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
		 << " cannot be registered ---- electron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsElectronPenelope(name) );
	  electronIsRegistered = true;
	}
      
      // Register standard processes for positrons
    } else if (name == "positron-standard") 
    {
      if (positronIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- positron List already existing"                  
		 << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsPositronStandard(name) );
	  positronIsRegistered = true;
	}
      
      // Register penelope processes for positrons
    } else if (name == "positron-penelope") 
    {
      if (positronIsRegistered) 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name  
		 << " cannot be registered ---- positron List already existing"   << G4endl;
	} 
      else 
	{
	  G4cout << "GmEMPhysics::ReplacePhysicsList: " << name 
                 << " is registered" << G4endl;
	  RegisterPhysics( new GmPhysicsPositronPenelope(name) );
	  positronIsRegistered = true;
	}

  } else {
    bFound = false;
  }  
  
  return bFound;
}


void GmEMPhysics::SetCuts()
{  
  // Set the threshold of production equal to the defaultCutValue
  // in the experimental set-up
  G4VUserPhysicsList::SetCutsWithDefault();
    
  if (verboseLevel>0) DumpCutValuesTable();
}

     
void GmEMPhysics::ConstructProcess()
{
  AddTransportation();

  G4String name;
  if (!gammaIsRegistered ) {
    name = "gamma-lowener";
    G4cout << "GmEMPhysics PhysicsList:DEFAULT = " << name << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsGammaLowEner(name) );
    gammaIsRegistered = true;
  } 
  
  if (!electronIsRegistered) {  
    name = "electron-lowener";
    G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsElectronLowEner(name) );
    electronIsRegistered = true;
  }

  if (!positronIsRegistered) {
    name = "positron-standard"; 
    G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
    RegisterPhysics( new GmPhysicsPositronStandard(name) );
    positronIsRegistered = true;
  }


  /* t G4PhysConstVector::iterator itr;
  for (itr = physicsVector->begin()WLWLS; itr!= physicsVector->end(); ++itr) {
    (*itr)->ConstructProcess();
    } */

  //  G4EmProcessOptions popt;
  // popt.SetApplyCuts(true);

  // Deexcitation
  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  if( G4bool(theParamMgr->GetNumericValue("GmEMPhysics:AtomicDeexcitation",1)) ){
    G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    G4bool bFluo = theParamMgr->GetNumericValue("AtomicDeexcitation:Fluorescence",1);
    G4bool bAuger = theParamMgr->GetNumericValue("AtomicDeexcitation:Auger",1);
    G4bool bPIXE = theParamMgr->GetNumericValue("AtomicDeexcitation:PIXE",0);
    if( bFluo) de->SetFluo(true);
    if( bAuger ) de->SetAuger(true);
    if( bPIXE ) de->SetPIXE(true);

    std::vector<G4String> regions;
    regions = theParamMgr->GetVStringValue("AtomicDeexcitation:Regions",regions); 
    if( regions.size() == 0 ) regions.push_back("DefaultRegionForTheWorld");
   for( size_t ii = 0; ii < regions.size(); ii++ ){
      de->SetDeexcitationActiveRegion(regions[ii], 1, bAuger, bPIXE );
    }
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);
  }
  
  G4VModularPhysicsList::ConstructProcess();
}
                                                                                
