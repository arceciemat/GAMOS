//#include "GamosCore/GamosGeometry/include/GmGeometryFromText.hh"

#include "GmRunManager.hh"
#include "GmRunMessenger.hh"
#include "GmFactoriesMessenger.hh"
#include "GmDeprecatedCommandsMessenger.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryMessenger.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmExtraPhysicsMessenger.hh"
#include "GamosCore/GamosPhysics/Cuts/include/GmUserLimitsMessenger.hh"
#include "GamosCore/GamosPhysics/Biasing/include/GmBiasingMessenger.hh"
#include "GamosCore/GamosPhysics/VarianceReduction/include/GmVarianceReductionMessenger.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMessenger.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"
#include "GamosCore/GamosSD/include/GmSDMessenger.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

#include "MagFieldManager/include/MagneticFieldModelManager.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4EmParameters.hh"
#include "G4DNAChemistryManager.hh"
#include "G4Scheduler.hh"
#include "G4UserTimeStepAction.hh"
#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "SEAL_Foundation/PluginManager/PluginManager/PluginManager.h"
#include "GamosCore/GamosGeometry/include/GmGeometryFactory.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorFactory.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTimeStepActionFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#endif

#include "CLHEP/Random/RandomEngine.h"

#include <sstream>

GmRunManager* GmRunManager::theRunManager = 0;
//----------------------------------------------------------------------
GmRunManager* GmRunManager::GetRunManager()
{
  return theRunManager; 
}

//----------------------------------------------------------------------
GmRunManager::GmRunManager()
{
  if(theRunManager) {
    G4Exception("GmRunManager::GmRunManager",
		"Error",
		FatalException,
		"Constructed twice."); 
  }

  theRunManager = this;

  new GmFactoriesMessenger( this );

  new GmDeprecatedCommandsMessenger;

  theGeometryName = "";
  thePhysicsListName = "";
  theGeneratorName = "";
  
  InitialiseFactories();

  GmParameterMgr::GetInstance();
  theGmSDMessenger = new GmSDMessenger();
  theGmAnalysisMessenger =  new GmAnalysisMessenger();
  theGmRunMessenger = new GmRunMessenger(this);

  new GmBaseMessenger();

  new GmGeometryMessenger();

  new GmExtraPhysicsMessenger();
  new GmUserLimitsMessenger();
  new GmBiasingMessenger();
  new GmVarianceReductionMessenger();

  new GmScoringMessenger();

  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("  MagneticFieldModelManager:On",0)) ) {
    MagneticFieldModelManager::GetInstance();
  }

  bRestoreSeedsUsed = false;

	G4EmParameters::Instance();
}

//----------------------------------------------------------------------
void GmRunManager::CreateG4RunManager()
{
  theG4RunManager = new G4RunManager;
  theG4RunManager->SetVerboseLevel(1);
  G4cout << " Using Random Engine: " << CLHEP::HepRandom::getTheEngine()->name() << G4endl; 

}

//----------------------------------------------------------------------
GmRunManager::~GmRunManager()
{
  //  delete theG4RunManager;
  delete theGmRunMessenger;
  GmAnalysisMgr::DeleteInstances();
  delete theGmSDMessenger;
  delete theGmAnalysisMessenger;

  GmParameterMgr::GetInstance()->CheckParametersUsage();

  delete GmUserActionMgr::GetInstance();

  delete GmScoringMgr::GetInstance();

  //--- Check if too many random numbers are used 
#ifndef WIN32
  G4int nCalls =  CLHEP::HepRandomEngine::theNCalls;
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmRunManager:PrintNRandomNumbersUsed",0) ) ){
    G4int nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
    G4cout << "@@@ Number of random numbers used: " << nCalls << " / " << nev << " = " << nCalls/nev << " numbers/event" << G4endl;
  }
  if(bRestoreSeedsUsed && nCalls > theNSeedsInFile ) {
    G4Exception("GmRunManager::~GmRunManager",
		"Warning",
		JustWarning,
		G4String("Too many random numbers used: " + GmGenUtils::itoa(nCalls) + ", more than those in file: " + GmGenUtils::itoa(theNSeedsInFile) + " There is a very small chance of random number repetition ").c_str());
    G4int nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
    G4cout << "Number of random numbers used per event= " << nCalls/nev << G4endl;
  }
#endif


}

//----------------------------------------------------------------------
void GmRunManager::InitialiseFactories()
{
#ifndef ROOT5
  seal::PluginManager::get()->initialise();
#endif
}


//----------------------------------------------------------------------
void GmRunManager::Initialise()
{
  theG4RunManager->Initialize();

}


//----------------------------------------------------------------------
void GmRunManager::SelectGeometry( const G4String& name )
{  
  if( theGeometryName != "" ) {
    G4cerr << "!!WARNING: selecting detector " << name << " while detector " << theGeometryName << " was previously selected " << G4endl;
  }

#ifdef ROOT5
  G4VUserDetectorConstruction* theGeometry = Reflex::PluginService::Create<G4VUserDetectorConstruction*>(name);
#else
  G4VUserDetectorConstruction* theGeometry = GmGeometryFactory::get()->create(name);
#endif
  
  if( theGeometry == 0 ) {
    G4cerr << "!!ERROR: detector " << name << " does not exist, program will crash if another detector is not selected " << G4endl;
  } else {
    theGeometryName = name;
    theG4RunManager->SetUserInitialization( theGeometry );
  }

}

//----------------------------------------------------------------------
void GmRunManager::SelectPhysicsList( const G4String& name )
{
  if( thePhysicsListName != "" ) {
    G4cerr << "!!WARNING: selecting physics list " << name << " while physics list " << thePhysicsListName << " was previously selected " << G4endl;
  }

#ifdef ROOT5
  Reflex::PluginService::SetDebug(21);  
   G4VUserPhysicsList* thePhysicsList = Reflex::PluginService::Create<G4VUserPhysicsList*>(name);
#else
   G4VUserPhysicsList* thePhysicsList = GmPhysicsFactory::get()->create(name);
#endif
  if( thePhysicsList == 0 ) {
    G4cerr << "!!ERROR: physics list " << name << " does not exist, program will crash if another physics list is not selected " << G4endl;
  } else {
    theG4RunManager->SetUserInitialization( thePhysicsList );
    thePhysicsListName = name;
  }

  //---- Register GmScoringUA to print scorers at end of run
  SelectUserAction("GmScoringUA");

}

//----------------------------------------------------------------------
void GmRunManager::SelectGenerator( const G4String& name )
{
  if( theGeneratorName != "" ) {
    G4cerr << "!!WARNING: selecting generator " << name << " while generator " << theGeneratorName << " was previously selected " << G4endl;
  }

#ifdef ROOT5
  Reflex::PluginService::SetDebug(21);  
  G4VUserPrimaryGeneratorAction* theGenerator = Reflex::PluginService::Create<G4VUserPrimaryGeneratorAction*>(name); 
#else
  G4VUserPrimaryGeneratorAction* theGenerator = GmGeneratorFactory::get()->create(name);
#endif
  if( theGenerator == 0 ) {
    G4cerr << "!!ERROR: generator " << name << " does not exist, program will crash if another generator is not selected " << G4endl;
  } else {
    theGeneratorName = name;
    theG4RunManager->SetUserAction( theGenerator );
  }
}

//----------------------------------------------------------------------
void GmRunManager::SelectUserAction( const G4String& values )
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( values );
  G4String name = wl[0];
  //-  G4cout << " GmRunManager::selectUserAction "<< name << G4endl;
#ifdef ROOT5
  Reflex::PluginService::SetDebug(21);  
  GmUserAction* userAction = Reflex::PluginService::Create<GmUserAction*>(name);
#else
  GmUserAction* userAction = GmUserActionFactory::get()->create(name);
#endif
  if( userAction == 0 ) {
    G4Exception("GmRunManager::SelectUserAction",
		"user action does not exist",
		FatalErrorInArgument,
		G4String(" User action " + name).c_str());
  } else {
    userAction->SetName( name );
//    if( UAVerb(infoVerb) ) G4cout << " GmRunManager::SelectUserAction user action added: " << name << G4endl;
    GmUserActionMgr::GetInstance()->CheckIfRepeated( name );
    for( unsigned int ii = 1; ii < wl.size(); ii++ ){
      userAction->SetFilterOrClassifier(wl[ii]);
    }
    userAction->InitialiseHistoNames();
  }
}

//----------------------------------------------------------------------
void GmRunManager::SelectUserTimeStepAction( const G4String& values )
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( values );
  G4String name = wl[0];
  //-  G4cout << " GmRunManager::selectUserAction "<< name << G4endl;
#ifdef ROOT5
  Reflex::PluginService::SetDebug(21);  
  G4UserTimeStepAction* userAction = Reflex::PluginService::Create<GmUserTimeStepAction*>(name);
#else
  G4UserTimeStepAction* userAction = GmUserTimeStepActionFactory::get()->create(name);
#endif
  if( userAction == 0 ) {
    G4Exception("GmRunManager::SelectUserTimeStepAction",
		"user action does not exist",
		FatalErrorInArgument,
		G4String(" User action " + name).c_str());
  } else {
    //    userAction->SetName( name );
//    if( UAVerb(infoVerb) ) G4cout << " GmRunManager::SelectUserAction user action added: " << name << G4endl;
    GmUserActionMgr::GetInstance()->CheckIfRepeated( name );
    /*    for( unsigned int ii = 1; ii < wl.size(); ii++ ){
      userAction->SetFilterOrClassifier(wl[ii]);
    }
    userAction->InitialiseHistoNames(); */
  }
   if(G4DNAChemistryManager::IsActivated())  {
     G4Scheduler::Instance()->
       SetUserAction(userAction);
   } else {
     G4Exception("GmRunManager::SelectUserTimeStepAction",
		 "",
		 FatalException,
		 "G4DNAChemistryManager is not activated, check your physics list");
   }
//stop at this time
}


//----------------------------------------------------------------------
void GmRunManager::SelectVerbosity( const G4String& name )
{
#ifndef GAMOS_NO_VERBOSE
  //  G4cout << " GmRunManager::selectVerbosity "<< name << G4endl;
#endif
  G4String verbName;
  G4String verbLevel;
  std::istringstream is((char*)name.c_str());
  is >> verbName >> verbLevel;

  G4int val = 4;
  if( GmGenUtils::IsNumber( verbLevel ) ) {
    val = atoi( verbLevel );
  } else {
    if( verbLevel == "silent" ) {
      val = silentVerb;
    } else if( verbLevel == "error" ) {
      val = errorVerb;
    } else if( verbLevel == "warning" ) {
      val = warningVerb;
    } else if( verbLevel == "info" ) {
      val = infoVerb;
    } else if( verbLevel == "debug" ) {
      val = debugVerb;
    } else if( verbLevel == "test" ) {
      val = testVerb;
    } else {
      G4Exception("GmRunManager::SelectVerbosity",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("invalid value = " + verbLevel).c_str());
    }
 }
  
  GmVVerbosity::SetVerbosityLevel( verbName, val );
}

//---------------------------------------------------------------------
void GmRunManager::SetRandomSeeds( G4int index, G4int nTries )
{
  // init
  if (index==0&&nTries==0) {
    index = 1; // CMSTime().seconds();
    nTries = 1; //getpid() % 50;
  }
  
  index = abs(index);
  //  index = index % 215;
  
  G4cout << "Gamos Random: initializing seeds from table at position " 
		 << index <<  " and throwing " << nTries << "+1 numbers" << G4endl;

  long seeds[2];
  seeds[0] = index*index;
  seeds[1] = index*index*nTries;

  G4cout << " GamosRandom: seeds before nTries " << seeds[0] << " " << seeds[1] << G4endl;

  CLHEP::HepRandom::setTheSeeds(seeds);
  //  G4cout << "Gamos Random: after seeds " <<  hran.flat() << G4endl;

  //  int nTriesOrig = nTries;
  float l;
  CLHEP::HepRandom hran; 
  while(nTries-- >= 0) { 
    l = hran.flat();
  }
  G4cout << "Gamos Random: First random number is " << l << "\n" << G4endl;

}

#include <sstream>
//---------------------------------------------------------------------
void GmRunManager::RestoreRandomSeeds( G4int index, G4int nTries )
{
  CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
  if( CLHEP::HepRandom::getTheEngine()->name() != "HepJamesRandom" ) {
    G4Exception("GmRunManager::RestoreRandomSeeds",
		"",
		FatalException,
		("/gamos/random/restoreSeeds can only be used with random engine HepJamesRandom, while you are using " + CLHEP::HepRandom::getTheEngine()->name()).c_str());
  }
  bRestoreSeedsUsed = true;
  theNSeedsInFile = GmParameterMgr::GetInstance()->GetNumericValue("GmRunManager:RandomSeedsInFile",1.e11);
  G4String seedFileDir = GmParameterMgr::GetInstance()->GetStringValue("GmRunManager:SeedFileDir","initialSeeds");

  //--- 
  std::ostringstream nseedS;
  nseedS << std::setprecision(2) << theNSeedsInFile;

#ifdef WIN32
  // convert 1e+010 in 1e+10
  G4String nseedSs = nseedS.str().substr(0,3) + nseedS.str().substr(4,2);
  G4String filePrefix = seedFileDir + "\\initialSeeds." + nseedSs + ".";
#else
  G4String filePrefix = seedFileDir + "/initialSeeds." + nseedS.str() + ".";
#endif
  G4String numStr = GmGenUtils::itoa(index);
  /*  if( CLHEP::HepRandom::getTheEngine()->name() == "MixMaxRng" ) { 
    if( index < 10 ) {
      numStr = "0000"+numStr;
    } else if( index < 100 ) {
      numStr = "000"+numStr;
    } else if( index < 1000 ) {
      numStr = "00"+numStr;
    } else if( index < 10000 ) {
      numStr = "0"+numStr;
    }
  } */
 
  G4String fileName = filePrefix + numStr;
  
  char* pathc = getenv( "GAMOS_SEARCH_PATH" );
  if( !pathc ) G4Exception("GmRunManager::RestoreRandomSeeds",
                           "Wrong argument",
                           FatalErrorInArgument,
                           "No GAMOS_SEARCH_PATH variable defined, please defin\
e it as in config/gamosconf.sh");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  fileName = GmGenUtils::FileInPath( path, fileName );
  CLHEP::HepRandom::restoreEngineStatus(fileName);
  //  CLHEP::HepRandom::showEngineStatus();
  G4cout << "Gamos Random: restoring seeds from file " 
		 << fileName <<  " and throwing " << nTries << "+1 numbers" << G4endl;

  CLHEP::HepRandom hran; 
  float l;
  while(nTries-- >= 0) { 
    l = hran.flat();
  }

  G4cout << "Gamos Random: First random number is " << l << "\n" << G4endl;

}
