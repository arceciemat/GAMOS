#include "GmG4HadronicPhysicsLists.hh"
#include "GmG4HadronicPhysicsListsMessenger.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

GmG4HadronicPhysicsLists::GmG4HadronicPhysicsLists() : G4VModularPhysicsList()
{
  SetDefaultCutValue(0.7*CLHEP::mm);
  verboseLevel = 1;

  fMessenger = new GmG4HadronicPhysicsListsMessenger(this);

  // Particles
  fParticleList = new G4DecayPhysics(verboseLevel);

  // EM physics
  fEmPhysicsList = new G4EmStandardPhysics_option3(verboseLevel);

  bPhysicsReplaced = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

GmG4HadronicPhysicsLists::~GmG4HadronicPhysicsLists()
{
  delete fMessenger;
  delete fParticleList;
  delete fEmPhysicsList;
  for(size_t i=0; i<fHadronPhys.size(); i++) {
    delete fHadronPhys[i];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::ConstructParticle()
{
  fParticleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::ConstructProcess()
{
  if( !bPhysicsReplaced ) {
    ReplacePhysicsList("QGSP_BIC_HP_EMY");
    bPhysicsReplaced = false; // using default value, allow for another replace
  }
  
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  fParticleList->ConstructProcess();
  for(size_t i=0; i<fHadronPhys.size(); i++) {
    fHadronPhys[i]->ConstructProcess();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::ReplacePhysicsList(const G4String& name)
{
  if( bPhysicsReplaced ) {
    G4Exception("GmG4HadronicPhysicsLists::ReplacePhysicsList",
		"",
		FatalException,
		"/gamos/GmG4HadronicPhysics/replacePhysics can be used only once");
  }
  bPhysicsReplaced = true;
  
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) {
    G4cout << "GmG4HadronicPhysicsLists::ReplacePhysicsList: <" << name << ">" << G4endl;
  }
#endif
  size_t lastUS = name.find_last_of('_');
  if( lastUS == std::string::npos ) {
    G4Exception("GmG4HadronicPhysicsLists::ReplacePhysicsList",
		"",
		FatalException,
		("Physics list option does not exist: "+name).c_str());
  }
  G4String emPhysName = name.substr(lastUS + 1);

  if (emPhysName == "emOpt0" || emPhysName == "EMU") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics(verboseLevel);
    
  } else if (emPhysName == "emOpt1" || emPhysName == "EMV") {
    
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1(verboseLevel);
    
  } else if (emPhysName == "emOpt2" || emPhysName == "EMX") {
    
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2(verboseLevel);
    
  } else if (emPhysName == "emOpt3" || emPhysName == "EMY") {
    
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3(verboseLevel);
    
  } else if (emPhysName == "emOpt4" || emPhysName == "EMZ") {

    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
    
  } else if (emPhysName == "stdGS") {
    
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsGS(verboseLevel);

  } else if (emPhysName == "Liv") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePhysics();

  } else if (emPhysName == "LivPolar") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLivermorePolarizedPhysics();

  } else if (emPhysName == "lowEP") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmLowEPPhysics();

  } else if (emPhysName == "Pen") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmPenelopePhysics();

  } else if (emPhysName == "stdGS") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsGS(verboseLevel);

  } else if (emPhysName == "stdWVI") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsWVI(verboseLevel);

  } else if (emPhysName == "stdSS") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysicsSS(verboseLevel);
    
  } else { 
    G4Exception("GmG4HadronicPhysicsLists::ReplacePhysicsList",
		"",
		FatalException,
		("EM physics list option does not exist: "+emPhysName).c_str());
  }

  G4String hadPhysName = name.substr(0,lastUS);
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) {
    G4cout << "GmG4HadronicPhysicsLists::ReplacePhysicsList: emPhysName=" << emPhysName << " hadPhysName=" << hadPhysName << " " << lastUS << G4endl;
  }
#endif

  if (hadPhysName == "FTFP_BERT") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT(verboseLevel));

  } else if (hadPhysName == "FTFP_BERT_TRV") {

    SetBuilderList1(false);
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT_TRV(verboseLevel));

  } else if (hadPhysName == "FTF_BIC") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsFTF_BIC(verboseLevel));

  } else if (hadPhysName == "QBBC") {

    SetBuilderList2();
    fHadronPhys.push_back( new G4HadronInelasticQBBC(verboseLevel));

  } else if (hadPhysName == "QGSP_BERT") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT(verboseLevel));

  } else if (hadPhysName == "QGSP_FTFP_BERT") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_FTFP_BERT(verboseLevel));

  } else if (hadPhysName == "QGSP_BERT_HP") {

    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT_HP(verboseLevel));
    fHadronPhys.push_back( new G4RadioactiveDecayPhysics(verboseLevel));

  } else if (hadPhysName == "QGSP_BIC") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC(verboseLevel));
    fHadronPhys.push_back( new G4IonElasticPhysics(verboseLevel));

  } else if (hadPhysName == "QGS_BIC") {

    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsQGS_BIC(verboseLevel));

  } else if (hadPhysName == "QGSP_BIC_HP") {

    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP(verboseLevel));
    fHadronPhys.push_back( new G4RadioactiveDecayPhysics(verboseLevel));

  } else if (hadPhysName == "QGSP_BIC_AllHP") {

    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_AllHP(verboseLevel));
    fHadronPhys.push_back( new G4RadioactiveDecayPhysics(verboseLevel));

  } else if (hadPhysName == "QGSP_INCLXX") {

    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX(verboseLevel));
    fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel));
    
  } else if (hadPhysName == "QGSP_INCLXX_HP") {
    
    SetBuilderList0(true);
    //    G4HadronPhysicsINCLXX(const G4String& name, const G4bool quasiElastic=true, const G4bool neutronHP=false, const G4bool ftfp=false);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX("G4HadronPhysicsINCLXX",true,true,false));
    fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel));
    
  } else if (hadPhysName == "FTFP_INCLXX") {
    
    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX("G4HadronPhysicsINCLXX",true,false,true));
    fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel));

  } else if (hadPhysName == "FTFP_INCLXX_HP") {

    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX("G4HadronPhysicsINCLXX",true,true,true));
    fHadronPhys.push_back( new G4IonINCLXXPhysics(verboseLevel));

  } else {

    G4Exception("GmG4HadronicPhysicsLists::ReplacePhysicsList",
		"",
		FatalException,
		("Had physics list option does not exist: "+hadPhysName).c_str());
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::SetBuilderList0(G4bool flagHP)
{
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
  if(!flagHP) {
    fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::SetBuilderList1(G4bool flagHP)
{
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    fHadronPhys.push_back( new G4HadronHElasticPhysics(verboseLevel) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
  if(!flagHP) {
    fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::SetBuilderList2()
{
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  fHadronPhys.push_back( new G4HadronElasticPhysicsXS(verboseLevel) );
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysicsXS(verboseLevel));
  fHadronPhys.push_back( new G4IonElasticPhysics(verboseLevel));
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void GmG4HadronicPhysicsLists::List()
{
  G4cout << "### GmG4HadronicPhysicsLists available: FTFP_BERT FTFP_BERT_EMV "
         << "FTFP_BERT_EMX FTFP_BERT_EMZ FTFP_BERT_TRV"
         << G4endl;
  G4cout << "                            FTF_BIC QBBC QGSP_BERT "
         << "QGSP_BERT_EMV QGSP_BERT_EMX"
         << G4endl; 
  G4cout << "                            QGSP_BERT_HP QGSP_FTFP_BERT "
         << "QGSP_FTFP_BERT_EMV"
         << G4endl; 
  G4cout << "                            QGS_BIC QGSP_BIC QGSP_BIC_EMY "
         << "QGSP_BIC_HP" 
         << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

