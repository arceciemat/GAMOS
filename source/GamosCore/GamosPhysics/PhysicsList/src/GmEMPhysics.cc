#include "GmEMPhysics.hh"
#include "GmEMPhysicsMessenger.hh"
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
#include "GmPhysicsGammaStandard_XSChange.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


GmEMPhysics::GmEMPhysics(): G4VModularPhysicsList()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 0.1 * CLHEP::mm;
  messenger = new GmEMPhysicsMessenger(this);
  SetVerboseLevel(1);
  ConstructParticles();


  G4String name;
  name = "gamma-standard";
  G4cout << "GmEMPhysics PhysicsList:DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsGammaStandard(name,22) );

  name = "electron-standard";
  G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsElectronStandard(name, 11) );

  name = "positron-standard"; 
  G4cout << "GmEMPhysics PhysicsList: DEFAULT = " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsPositronStandard(name, 111) );
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
  G4MuonMinus::MuonMinusDefinition();
  G4MuonPlus::MuonPlusDefinition();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    particleIterator->value();
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

  // Replace standard processes for gammas
  if (name == "gamma-standard") {
    ReplacePhysics( new GmPhysicsGammaStandard(name,22) );

  } else if (name == "gamma-lowener" || name == "gamma-epdl") {
    ReplacePhysics( new GmPhysicsGammaLowEner(name,22) );

  } else if (name == "gamma-penelope") {
    ReplacePhysics( new GmPhysicsGammaPenelope(name,22) );
  
  } else if (name == "electron-standard") {
    ReplacePhysics( new GmPhysicsElectronStandard(name,11) );

  } else if (name == "electron-lowener" || name == "electron-eedl") {
    ReplacePhysics( new GmPhysicsElectronLowEner(name,11) );
      
  } else if (name == "electron-penelope") {
    ReplacePhysics( new GmPhysicsElectronPenelope(name,11) );
      
  } else if (name == "positron-standard") {
    ReplacePhysics( new GmPhysicsPositronStandard(name,111) );

  } else if (name == "positron-penelope") {
    ReplacePhysics( new GmPhysicsPositronPenelope(name,111) );

  } else if (name == "gamma-standard_XSChange") {
    ReplacePhysics( new GmPhysicsGammaStandard_XSChange(name,22) );

  } else {
    bFound = false;
  }  

  if( bFound ) G4cout << "GmEMPhysics::ReplacePhysicsList: " << name << " is registered" << G4endl;

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
  //  AddTransportation();


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
