#include "GmEMStandardPhysics.hh"
#include "GmEMStandardPhysicsMessenger.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsSS.hh"

#include "G4UnitsTable.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Hadrons
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

#include "G4SeltzerBergerModel.hh"
#include "G4ModifiedTsai.hh"
#include "G4Generator2BN.hh"
#include "G4Generator2BS.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmEMStandardPhysics::GmEMStandardPhysics() : G4VModularPhysicsList()
{
  pMessenger = new GmEMStandardPhysicsMessenger(this); 
   
  // EM physics
  emName = G4String("local");
  emPhysics = new G4EmStandardPhysics_option3();
  RegisterPhysics(emPhysics);

  defaultCutValue = 1.*CLHEP::mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEMStandardPhysics::~GmEMStandardPhysics()
{
  delete emPhysics;
  delete pMessenger;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEMStandardPhysics::ConstructParticle()
{
// pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
// gamma
  G4Gamma::GammaDefinition();
  
// optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();

// leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();  

// mesons
  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

// barions
  G4BaryonConstructor bConstructor;
  bConstructor.ConstructParticle();

// ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEMStandardPhysics::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
//  AddTransportation();
//  emPhysics->ConstructProcess();
  AddDecay();  
  SelectBremssAngularDist();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void GmEMStandardPhysics::AddDecay()
{
  // Add Decay Process

  G4Decay* fDecayProcess = new G4Decay();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (fDecayProcess->IsApplicable(*particle) && !particle->IsShortLived()) { 

      pmanager ->AddProcess(fDecayProcess);

      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);

    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEMStandardPhysics::ReplacePhysicsList(const G4String& name)
{
  if (verboseLevel>-1) {
    G4cout << "GmEMStandardPhysics::AddGmEMStandardPhysics: <" << name << ">" << G4endl;
  }

  if (name == emName) return;

  if (name == "emstandard_opt0"
      || name == "emstandard_option0") {
    
    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysics();

  } else if (name == "emstandard_opt1" 
	     || name == "emstandard_option1") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysics_option1();
    
  } else if (name == "emstandard_opt2"
    || name == "emstandard_option2") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysics_option2();
    
  } else if (name == "emstandard_opt3"
	     || name == "emstandard_option3") {
    
    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysics_option3();

  } else if (name == "emstandard_opt4"
	     || name == "emstandard_option4") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysics_option4();
    
  } else if (name == "emlivermore") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmLivermorePhysics();
    
  } else if (name == "emlivermorepolarized") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmLivermorePolarizedPhysics();
    
  } else if (name == "emlowEP") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmLowEPPhysics();
    
  } else if (name == "empenelope") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmPenelopePhysics();
    
  } else if (name == "emstandard_GS") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysicsGS();
    
  } else if (name == "emstandard_WVI") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysicsWVI();
    
  } else if (name == "emstandard_SS") {

    emName = name;
    delete emPhysics;
    emPhysics = new G4EmStandardPhysicsSS();
    
  } else {

    G4cout << "GmEMStandardPhysics::AddGmEMStandardPhysics: <" << name << ">"
           << " is not defined"
           << G4endl;
	  exit(1);
  }

}

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

//----------------------------------------------------------------------
void GmEMStandardPhysics::SelectBremssAngularDist()
{
  G4SeltzerBergerModel* bremsModel = 0;

  G4String theProcessName = "eBrems";

  //  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  //  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
   auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = procVector->size()-1; ii >= 0; ii-- ) {
      if( theProcessName == (*procVector)[ii]->GetProcessName() ) {
	G4String angularDist;
	G4VEmProcess* EMProcess = static_cast<G4VEmProcess*>((*procVector)[ii]);
	bremsModel = static_cast<G4SeltzerBergerModel*>(EMProcess->EmModel(0));
	if( particle == G4Electron::Electron()) {
	  angularDist = GmParameterMgr::GetInstance()->GetStringValue("GmPhysicsElectron:Bremsstrahlung:AngularDistribution","");
	} else if( particle == G4Positron::Positron()) {
	  angularDist = GmParameterMgr::GetInstance()->GetStringValue("GmPhysicsPositron:Bremsstrahlung:AngularDistribution","");
	}	
	
	if( angularDist == "tsai" ) {
	  bremsModel->SetAngularDistribution( new G4ModifiedTsai() );
	}else if( angularDist == "2bn" ) {
	  bremsModel->SetAngularDistribution( new G4Generator2BN() );
	}else if( angularDist == "2bs" ) {
	  bremsModel->SetAngularDistribution( new G4Generator2BS() );
	}else if( angularDist == "" ) {
	} else {
	  G4Exception("GmVPhysicsElectron::SelectBremssAngularDist",
		      "Wrong angular distribution",
		      FatalErrorInArgument,
		      G4String("Available distributions are tsai / 2bn /2bs , you have set it to "+angularDist).c_str());
	}
      }
    }
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmEMStandardPhysics::SetCuts()
{
      
  if (verboseLevel >0){
    G4cout << "GmEMStandardPhysics::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}
