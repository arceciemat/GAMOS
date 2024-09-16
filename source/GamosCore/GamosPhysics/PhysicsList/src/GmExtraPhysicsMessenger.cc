#include "GmExtraPhysicsMessenger.hh"
#include "GmPhysicsVerbosity.hh"
#include "GmPhysicsParallel.hh"
#include "GmPhysicsXrayRefraction.hh"
#include "GmPhysicsDecay.hh"
#include "GmPhysicsRadioactiveDecay.hh"
#include "GmPhysicsOpticalPhoton.hh"
#include "GmPhysicsElectronNuclear.hh"
#include "GmPhysicsPositronNuclear.hh"
#include "GmPhysicsGammaNuclear.hh"
#include "GmPhysicsCerenkov.hh"
#include "GmPhysicsCoulombScattering.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4EmModelManager.hh"
#include "G4EmParameters.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4RadioactiveDecay.hh"

#include "G4PEEffectFluoModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4PenelopePhotoElectricModel.hh"

#include "G4KleinNishinaCompton.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4PenelopeComptonModel.hh"
#include "G4LowEPComptonModel.hh"

#include "G4BetheHeitlerModel.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4BetheHeitler5DModel.hh"

#include "GmLivermoreRayleighModel_XSChangeNEW.hh"
#include "G4PenelopeRayleighModel.hh"

#include "G4MollerBhabhaModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4eIonisation.hh"
#include "G4EmStandUtil.hh"

#include "G4SeltzerBergerModel.hh"
#include "G4eBremsstrahlungRelModel.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

#include "G4eeToTwoGammaModel.hh"
#include "G4PenelopeAnnihilationModel.hh"

#include "G4UrbanMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"

#include "G4RegionStore.hh"
#include "G4IonConstructor.hh"
#include "G4OpticalPhoton.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
#include "G4VMultipleScattering.hh"

#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"

#include <set>

// ----------------------------------------------------------------------------
GmExtraPhysicsMessenger::GmExtraPhysicsMessenger()
{  
  // apply cuts to all processes
  theApplyCutsCmd = new G4UIcmdWithABool("/gamos/physics/applyCutsForAllProcesses",this);  
  theApplyCutsCmd->SetGuidance("Apply production cuts to all processes");
  theApplyCutsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  // instantiate parallel scoring process
  theParallelPhysCmd = new G4UIcmdWithoutParameter("/gamos/physics/addParallelProcess",this);  
  theParallelPhysCmd->SetGuidance("Apply production cuts to all processes");
  theParallelPhysCmd->AvailableForStates(G4State_Idle);
  
  // add new physics
  theAddPhysicsCmd = new GmUIcmdWithAString("/gamos/physics/addPhysics",this);  
  theAddPhysicsCmd->SetGuidance("Add new physics: decay, radioactiveDecay, opticalphoton, cerenkov, gamma-nuclear, electron-nuclear, positron-nuclear, xray-refraction, CoulombScattering");
  theAddPhysicsCmd->AvailableForStates(G4State_Idle);

  // remove processes by type
  theRemoveProcessesByTypeCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByType",this);  
  theRemoveProcessesByTypeCmd->SetGuidance("Remove processes of given types: NotDefined, Transportation, Electromagnetic, Optical, Hadronic, Photolepton_hadron, Decay, General, Parameterisation, UserDefined");
  theRemoveProcessesByTypeCmd->AvailableForStates(G4State_Idle);
  
  // delete processes by name
  theRemoveProcessesCmd = new GmUIcmdWithAString("/gamos/physics/removeProcesses",this);  
  theRemoveProcessesCmd->SetGuidance("Remove processes of given name");
  theRemoveProcessesCmd->AvailableForStates(G4State_Idle);
  theRemoveProcessesByNameCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByName",this);  
  theRemoveProcessesByNameCmd->SetGuidance("Remove processes of given name");
  theRemoveProcessesByNameCmd->AvailableForStates(G4State_Idle);
  
  // delete processes by particle and name
  theRemoveProcessesByParticleCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByParticle",this);  
  theRemoveProcessesByParticleCmd->SetGuidance("Remove processes of given name for given particles: PARTICLE_1 PROCESS_1 PARTICLE_2 PROCESS_2 ...");
  theRemoveProcessesByParticleCmd->AvailableForStates(G4State_Idle);
  theRemoveProcessesByParticleAndNameCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByParticleAndName",this);  
  theRemoveProcessesByParticleAndNameCmd->SetGuidance("Remove processes of given name for given particles: PARTICLE_1 PROCESS_1 PARTICLE_2 PROCESS_2 ...");
  theRemoveProcessesByParticleAndNameCmd->AvailableForStates(G4State_Idle);

  // replace models of all processes to standard / low-energy / penelope
  theReplaceModelSetCmd = new GmUIcmdWithAString("/gamos/physics/replacePhysics",this);  
  theReplaceModelSetCmd->SetGuidance("Replace process model: gamma-standard gamma-lowener gamma-penelope electron-standard electron-lowener electron-penelope");
  theReplaceModelSetCmd->AvailableForStates(G4State_Idle);

  // replace one model of one process
  theReplaceModelCmd = new GmUIcmdWithAString("/gamos/physics/replaceProcessModel",this);  
  theReplaceModelCmd->SetGuidance("Replace process model: gamma-standard gamma-lowener gamma-penelope electron-standard electron-lowener electron-penelope");
  theReplaceModelCmd->AvailableForStates(G4State_Idle);

}


// ----------------------------------------------------------------------------
GmExtraPhysicsMessenger::~GmExtraPhysicsMessenger()
{
  delete theApplyCutsCmd;
  delete theParallelPhysCmd;
  delete theAddPhysicsCmd;
  delete theRemoveProcessesCmd;
  delete theRemoveProcessesByNameCmd;
  delete theRemoveProcessesByTypeCmd;
  delete theRemoveProcessesByParticleCmd;
  delete theRemoveProcessesByParticleAndNameCmd;
  delete theReplaceModelSetCmd;
  delete theReplaceModelCmd;
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == theApplyCutsCmd){
    G4EmParameters* empar = G4EmParameters::Instance();
    empar->SetApplyCuts(GmGenUtils::GetBoolean(newValue));
    
  } else if (command == theParallelPhysCmd){
    GmPhysicsParallel* parallelPhys = new GmPhysicsParallel;
    parallelPhys->ConstructProcess();
    
  } else if (command == theAddPhysicsCmd){
    AddPhysics( newValue );

  } else if (command == theRemoveProcessesCmd){
    RemoveProcessesByName(newValue);

  } else if (command == theRemoveProcessesByNameCmd){
    RemoveProcessesByName(newValue);

  } else if (command == theRemoveProcessesByTypeCmd){
    RemoveProcessesByType(newValue);

  } else if (command == theRemoveProcessesByParticleCmd){
    RemoveProcessesByParticleAndName(newValue);

  } else if (command == theRemoveProcessesByParticleAndNameCmd){
    RemoveProcessesByParticleAndName(newValue);

  } else if (command == theReplaceModelSetCmd){
    ReplaceModelSet(newValue);

  } else if (command == theReplaceModelCmd){
    ReplaceModel(newValue);

  } 

} 
 
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4TauMinus.hh"
#include "G4TauPlus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4NeutrinoTau.hh"
#include "G4AntiNeutrinoTau.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4Eta.hh"
#include "G4EtaPrime.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh" 

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::AddPhysics(G4String newValue)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValue );
  if( wl.size() != 1 ) {
    G4Exception("GmExtraPhysicsMessenger::AddPhysics",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("One and only one parameter in /gamos/physics/addPhysics command, there are " + GmGenUtils::itoa(wl.size())).c_str());
  }

  // instantiate X-ray refraction process
  if( newValue == "xray-refraction" ) {
    GmPhysicsXrayRefraction* xrayRefracPhys = new GmPhysicsXrayRefraction;
    xrayRefracPhys->ConstructProcess();

  // instantiate G4decay process
  } else if( newValue == "decay" ) {
    GmPhysicsDecay* decayPhys = new GmPhysicsDecay;
    decayPhys->ConstructProcess();
    
  // instantiate radioactive decay process
  } else if( newValue == "radioactiveDecay" ) {
    GmPhysicsRadioactiveDecay* radioPhys = new GmPhysicsRadioactiveDecay;
    radioPhys->ConstructProcess();
    
  // instantiate optical photon processes
  } else if( newValue == "opticalphoton" ) {
    GmPhysicsOpticalPhoton* optPhotPhys = new GmPhysicsOpticalPhoton;
    optPhotPhys->ConstructProcess();

  } else if( newValue == "gamma-nuclear" ) {
    GmPhysicsGammaNuclear* gammaPhys = new GmPhysicsGammaNuclear();
    gammaPhys->ConstructProcess();

  } else if( newValue == "electron-nuclear" ) {
    GmPhysicsElectronNuclear* elePhys = new GmPhysicsElectronNuclear();
    elePhys->ConstructProcess();

  } else if( newValue == "positron-nuclear" ) {
    GmPhysicsPositronNuclear* posiPhys = new GmPhysicsPositronNuclear();
    posiPhys->ConstructProcess();

  // instantiate G4decay process
  } else if( newValue == "cerenkov" ) {
    GmPhysicsCerenkov* cerenkovPhys = new GmPhysicsCerenkov;
    cerenkovPhys->ConstructProcess();
    
  } else if( newValue == "de-excitation" ) {
    AddDeexcitation();

  } else if( newValue.find("coulombScattering") != std::string::npos 
	     || newValue.find("CoulombScattering") != std::string::npos ) {
    GmPhysicsCoulombScattering* coulScat = 0;
    if( newValue.find("_") == std::string::npos ) {
      coulScat = new GmPhysicsCoulombScattering();
    } else {
      size_t is = newValue.find("_");
      G4double lowEnergyLimit = GmGenUtils::GetValue(newValue.substr(is+1,9999));
      coulScat = new GmPhysicsCoulombScattering(lowEnergyLimit);
    }
    coulScat->ConstructProcess();

  } else {
    G4Exception("GmExtraPhysicsMessenger::AddPhysics",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("ARGUMENT is = " + newValue + " .It may be: decay, radioactiveDecay, opticalphoton, cerenkov, gamma-nuclear, electron-nuclear, positron-nuclear, xray-refraction, de-excitation, CoulombScattering").c_str());
  }
  
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::RemoveProcessesByType(G4String )
{
  std::ostringstream message;
  message << "!!!ERROR:  /gamos/physics/removeProcessesByType is deprecated " << G4endl
	  << "Use instead  /process/inactivate name (particle) " << G4endl
	  << "  name:     process name or type name" << G4endl
	  << "  particle: particle name (all: for all particles)" << G4endl;
  G4Exception("GmExtraPhysicsMessenger::RemoveProcessesByType", "",
	      FatalException, message);
    
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::RemoveProcessesByName(G4String )
{
  std::ostringstream message;
  message << "!!!ERROR:  /gamos/physics/removeProcessesByName is deprecated " << G4endl
	  << "Use instead  /process/inactivate name (particle) " << G4endl
	  << "  name:     process name or type name" << G4endl
	  << "  particle: particle name (all: for all particles)" << G4endl;
  G4Exception("GmExtraPhysicsMessenger::RemoveProcessesByType", "",
	      FatalException, message);

}


// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::RemoveProcessesByParticleAndName(G4String )
{
  std::ostringstream message;
  message << "!!!ERROR:  /gamos/physics/removeProcessesByName is deprecated " << G4endl
	  << "Use instead  /process/inactivate name (particle) " << G4endl
	  << "  name:     process name or type name" << G4endl
	  << "  particle: particle name (all: for all particles)" << G4endl;
  G4Exception("GmExtraPhysicsMessenger::RemoveProcessesByType", "",
	      FatalException, message);

}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::ReplaceModelSet(G4String newValue)
{
  mmss thePartprocNames; 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValue );
  if( wl.size() < 1 ) {
    G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		"No argument given",
		FatalException,
		"Needs one argument: gamma-standard gamma-lowener gamma-penelope electron-standard electron-lowener electron-penelope positron-standard positron-penelope");
  }

  for( unsigned int ii = 0; ii < wl.size(); ii++ ){
    G4String newParticleModelName = wl[ii];
    size_t ig = newParticleModelName.find("-");
    G4String newParticleName = newParticleModelName.substr(0,ig);
    if( newParticleName == "electron" ) newParticleName = "e-"; //Geant4 name
    if( newParticleName == "positron" ) newParticleName = "e+"; //Geant4 name
    G4String newModelName = newParticleModelName.substr(ig+1,newParticleModelName.length());
    //    G4cout << " newParticleName " << newParticleName << " newModelName " << newModelName << G4endl;
    if( newParticleName == "gamma" ) {
      Replace1Model(newParticleName,"phot",newModelName);
      Replace1Model(newParticleName,"compt",newModelName);
      Replace1Model(newParticleName,"conv",newModelName);
      Replace1Model(newParticleName,"Rayl",newModelName);
    } else if( newParticleName == "e-" ) {
      Replace1Model(newParticleName,"eIoni",newModelName);
      Replace1Model(newParticleName,"eBrem",newModelName);
    } else if( newParticleName == "e+" ) {
      Replace1Model(newParticleName,"eIoni",newModelName);
      Replace1Model(newParticleName,"eBrem",newModelName);
      Replace1Model(newParticleName,"annihil",newModelName);
    }
  }
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::ReplaceModel(G4String newValue)
{
  mmss thePartprocNames; 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValue );
  if( wl.size() != 3 ) {
    G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		"No argument given",
		FatalException,
		"Needs one argument: gamma-standard gamma-lowener gamma-penelope electron-standard electron-lowener electron-penelope positron-standard positron-penelope");
  }
  
  G4String newParticleName = wl[0];
  G4String newProcessName = wl[1];
  G4String newModelName = wl[2];
  Replace1Model(newParticleName,newProcessName,newModelName);

}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::Replace1Model(G4String newParticleName, G4String newProcessName, G4String newModelName)
{
  //---- Look for process 
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  G4bool bParticleFound = false;
  G4bool bProcessFound = false;

  G4double CSlowEnergyLimit = 0.;  
  if( newModelName.find("CoulombScattering") != std::string::npos ) { // CoulombScat_100 sets low energy to 100, like in std_option4
    if( newModelName.find("_") != std::string::npos ) {
      size_t is = newModelName.find("_");
      CSlowEnergyLimit = GmGenUtils::GetValue(newModelName.substr(is+1,9999));
      newModelName = newModelName.substr(0,is);	      
    }	    
  }
  
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4String particleName = particle -> GetParticleName();
    //      G4cout << " particleName " << particleName << G4endl;
    if (particleName == newParticleName ) {
      bParticleFound = true;
      bProcessFound = false;
      if( newParticleName == "gamma" ) {	  
	//G4VEmModel* newModel2 = 0;
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = GmG4Utils::GetGammaProcessVector(pmanager->GetProcessList());

	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4VEmProcess* process = dynamic_cast<G4VEmProcess*>((*procVector)[ii]);
	  if( ! process ) { // Transportation
	    continue;
	  }
	  G4String processName = process->GetProcessName();
	  G4VEmModel* newModel = 0;
	  if( processName == newProcessName ) {
	    if( processName == "phot" ) {
	      bProcessFound = true;
	      if( newModelName == "standard" ) {
		newModel = new G4PEEffectFluoModel;
	      } else if( newModelName == "lowener" ) {
		newModel = new G4LivermorePhotoElectricModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopePhotoElectricModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong model given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "compt" ) {
	      bProcessFound = true;
	      if( newModelName == "standard" ) {
		newModel = new G4KleinNishinaCompton;
	      } else if( newModelName == "lowener" ) {
		newModel = new G4LivermoreComptonModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeComptonModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "lowEP" ) {
		newModel = new G4LowEPComptonModel();
		newModel->SetHighEnergyLimit(20*CLHEP::MeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong model given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "conv" ) { 
	      bProcessFound = true;
	      if( newModelName == "standard" ) {
		newModel = new G4BetheHeitlerModel;
	      } else if( newModelName == "lowener" ) {
		newModel = new G4LivermoreGammaConversionModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeGammaConversionModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "BetheHeitler5D" ) {
		newModel = new G4BetheHeitler5DModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong model given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else if( processName == "Rayl" ) { 
	      bProcessFound = true;
	      if( newModelName == "standard" ) {	   
	      } else if( newModelName == "lowener" ) {
		newModel = new GmLivermoreRayleighModel_XSChangeNEW();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeRayleighModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong model given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else {
	      continue;
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    // Delete old model and add new
	    std::vector<G4VEmModel*> models = process->modelManager->models;
#ifndef GAMOS_NO_VERBOSE
	    if( PhysicsVerb(debugVerb) ) {
	      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
		G4cout << ii2 << "OLD GAMMA MODELS " << models[ii2]->GetName() << G4endl; 
	      }
	    }
#endif
	    for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	      delete models[ii2];
	    }
	    process->modelManager->models.clear();
	    process->modelManager->flucModels.clear();
	    process->modelManager->regions.clear();
	    process->modelManager->orderOfModels.clear();
	    process->modelManager->isUsed.clear();
	    process->modelManager->nEmModels = 0;
	    process->AddEmModel(0,newModel);

#ifndef GAMOS_NO_VERBOSE
	    if( PhysicsVerb(debugVerb) ) {
	      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
		G4cout << ii2 << "NEW GAMMA MODELS " << models[ii2]->GetName() << G4endl; 
	      }
	    }
#endif
	  }
	}
      } else if( newParticleName == "e-" ) {
	G4VEmModel* newModel2 = 0;
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4String processName = (*procVector)[ii]->GetProcessName();
	  if( processName == newProcessName ) {
	    G4VEmModel* newModel = 0;
	    
	    if( processName == "eIoni" ) {
	      bProcessFound = true;
	      if( newModelName == "pen_std" ) {	
		G4eIonisation* eIoni = dynamic_cast<G4eIonisation*>((*procVector)[ii]);
		eIoni->SetFluctModel(G4EmStandUtil::ModelOfFluctuations());
	      }
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeIoni( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "eBrem" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeBrem( dynamic_cast<G4VEnergyLossProcess*>((*procVector)[ii]), newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "msc" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeMsc( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "CoulombScat" ) { // only change low energy limit
	      bProcessFound = true;
	      if ( CSlowEnergyLimit != 0. ) {
		G4CoulombScattering* procCS = dynamic_cast<G4CoulombScattering*>((*procVector)[ii]);
		procCS->SetMinKinEnergy(CSlowEnergyLimit);
		if( procCS->NumberOfModels() != 1 ) {
		}
		G4eCoulombScatteringModel* modelCS = dynamic_cast<G4eCoulombScatteringModel*>(procCS->EmModel(0));
		modelCS->SetLowEnergyLimit(CSlowEnergyLimit);
		modelCS->SetActivationLowEnergyLimit(CSlowEnergyLimit);
	      }
	    }
	    
	    if( newModel == 0 ) continue; // Rayleigh for standard models
	    
	    // Delete old model and add new
	    ReplaceG4Model((*procVector)[ii],newModel,newModel2);
	  }
	}
      } else if( newParticleName == "e+" ) {
	G4VEmModel* newModel2 = 0;
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4String processName = (*procVector)[ii]->GetProcessName();
	  if( processName == newProcessName ) {
	    G4VEmModel* newModel = 0;
	    
	    if( processName == "eIoni" ) {
	      bProcessFound = true;
	      if( newModelName == "pen_std" ) {
		G4eIonisation* eIoni = dynamic_cast<G4eIonisation*>((*procVector)[ii]);
		eIoni->SetFluctModel(G4EmStandUtil::ModelOfFluctuations());
	      }
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeIoni( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "eBrem" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeBrem( dynamic_cast<G4VEnergyLossProcess*>((*procVector)[ii]), newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "annihil" ) {
	      bProcessFound = true;
	      if( newModelName == "standard" ) {
		newModel = new G4eeToTwoGammaModel();
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeAnnihilationModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong model given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else if( processName == "msc" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeMsc( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "CoulombScat" ) { // only change low energy limit
	      bProcessFound = true;
	      if ( CSlowEnergyLimit != 0. ) {
		G4CoulombScattering* procCS = dynamic_cast<G4CoulombScattering*>((*procVector)[ii]);
		procCS->SetMinKinEnergy(CSlowEnergyLimit);
		if( procCS->NumberOfModels() != 1 ) {
		}
		G4eCoulombScatteringModel* modelCS = dynamic_cast<G4eCoulombScatteringModel*>(procCS->EmModel(0));
		modelCS->SetLowEnergyLimit(CSlowEnergyLimit);
		modelCS->SetActivationLowEnergyLimit(CSlowEnergyLimit);
	      }
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    
	    ReplaceG4Model((*procVector)[ii],newModel,newModel2);
	  }
	}
      } else if( newParticleName == "proton" 
		 || newParticleName == "mu-"
		 || newParticleName == "mu+"
		 || newParticleName == "kaon+"
		 || newParticleName == "kaon-" 
		 || newParticleName == "pi+"
		 || newParticleName == "pi-"
		 ) {
	G4VEmModel* newModel2 = 0;
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  //	  G4cout << ii << " " << particleName << " PROCESS LOOP " << (*procVector)[ii]->GetProcessName() << G4endl; //GDEB
	  G4String processName = (*procVector)[ii]->GetProcessName();
	  if( processName == newProcessName ) {
	    G4VEmModel* newModel = 0;
	    
	    if( processName == "hIoni" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeIoni( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "hBrem" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeBrem( dynamic_cast<G4VEnergyLossProcess*>((*procVector)[ii]), newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    } else if( processName == "msc" ) {
	      bProcessFound = true;
	      std::pair<G4VEmModel*,G4VEmModel*> newModels = ChangeMsc( newParticleName, newProcessName, newModelName );
	      newModel = newModels.first;
	      newModel2 = newModels.second;
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    
	    ReplaceG4Model((*procVector)[ii],newModel,newModel2);
	  }
	}
      } else {
	G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		    "Wrong particle given",
		    FatalException,
		    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	
      }  
      
      if( ! bProcessFound ) {
	G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		    "Wrong process given",
		    FatalException,
		    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
      }
    }
  }

  if( ! bParticleFound ) {
    G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		"Wrong particle given",
		FatalException,
		G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
  }

}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::AddDeexcitation()
{
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();

  G4bool bFluo = theParamMgr->GetNumericValue("AtomicDeexcitation:Fluorescence",1);
  G4bool bAuger = theParamMgr->GetNumericValue("AtomicDeexcitation:Auger",1);
  G4bool bPIXE = theParamMgr->GetNumericValue("AtomicDeexcitation:PIXE",0);
  de->SetFluo(bFluo);
  de->SetAuger(bAuger);
  de->SetPIXE(bPIXE);
  
  std::vector<G4String> regions;
  regions = theParamMgr->GetVStringValue("AtomicDeexcitation:Regions",regions); 
  if( regions.size() == 0 ) regions.push_back("WORLD");
  for( size_t ii = 0; ii < regions.size(); ii++ ){
    de->SetDeexcitationActiveRegion(regions[ii], 1, bAuger, bPIXE );
  }
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);

}
    
// ----------------------------------------------------------------------------
std::pair<G4VEmModel*,G4VEmModel*> GmExtraPhysicsMessenger::ChangeMsc(G4String newParticleName, G4String newProcessName, G4String newModelName)
{
  G4VEmModel* newModel = 0;
  G4VEmModel* newModel2 = 0;

  if( newModelName == "Urban" ) {
    newModel = new G4UrbanMscModel();
  } else if( newModelName == "WentzelVI" ) {
    newModel = new G4WentzelVIModel();
  } else if( newModelName == "GoudsmitSaunderson" ) {
    newModel = new G4GoudsmitSaundersonMscModel();
  } else if( newModelName == "GS_WVI" ) {
    newModel = new G4GoudsmitSaundersonMscModel();
    //    G4GoudsmitSaundersonMscModel* newModel = new G4GoudsmitSaundersonMscModel();
    // G4WentzelVIModel* newModel2 = new G4WentzelVIModel();
    newModel = new G4GoudsmitSaundersonMscModel();
    newModel2 = new G4WentzelVIModel();
    G4double highEnergyLimit = G4EmParameters::Instance()->MscEnergyLimit();
    newModel->SetHighEnergyLimit(highEnergyLimit);
    newModel2->SetLowEnergyLimit(highEnergyLimit);
  } else {
    G4Exception("void GmExtraPhysicsMessenger::ChangeMsc",
		"Wrong model given",
		FatalException,
		G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
  }

  return std::pair<G4VEmModel*,G4VEmModel*> (newModel,newModel2);
}

// ----------------------------------------------------------------------------
std::pair<G4VEmModel*,G4VEmModel*> GmExtraPhysicsMessenger::ChangeIoni(G4String newParticleName, G4String newProcessName, G4String newModelName)
{
  G4VEmModel* newModel = 0;
  G4VEmModel* newModel2 = 0;

  if( newModelName == "standard" ) {
    newModel = new G4MollerBhabhaModel;
  } else if( newModelName == "lowener" ) {
    newModel = new G4LivermoreIonisationModel();
  } else if( newModelName == "penelope" ) {
    newModel = new G4PenelopeIonisationModel();
  } else if( newModelName == "pen_std" ) {
    G4eIonisation* eioni = new G4eIonisation();
    eioni->SetFluctModel(G4EmStandUtil::ModelOfFluctuations());
    newModel = new G4PenelopeIonisationModel();
    newModel->SetHighEnergyLimit(0.1*CLHEP::MeV); 
    eioni->AddEmModel(0, newModel);
  } else {
    G4Exception("void GmExtraPhysicsMessenger::ChangeIoni",
		"Wrong model given",
		FatalException,
		G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
  }
  
  return std::pair<G4VEmModel*,G4VEmModel*> (newModel,newModel2);

}

// ----------------------------------------------------------------------------
std::pair<G4VEmModel*,G4VEmModel*> GmExtraPhysicsMessenger::ChangeBrem(G4VEnergyLossProcess* process, G4String newParticleName, G4String newProcessName, G4String newModelName)
{
  G4VEmModel* newModel = 0;
  G4VEmModel* newModel2 = 0;
  if( newModelName == "standard" ) {
    newModel = new G4SeltzerBergerModel();
    newModel2 = new G4eBremsstrahlungRelModel();
    newModel->SetLowEnergyLimit(process->MinKinEnergy());
    newModel->SetHighEnergyLimit(newModel2->LowEnergyLimit());
    newModel2->SetHighEnergyLimit(process->MaxKinEnergy());                
  } else if( newModelName == "lowener" ) {
    newModel = new G4LivermoreBremsstrahlungModel();
  } else if( newModelName == "penelope" ) {
    newModel = new G4PenelopeBremsstrahlungModel();
  } else {
    G4Exception("void GmExtraPhysicsMessenger::ChangeBrem",
		"Wrong model given",
		FatalException,
		G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
  }
  
  return std::pair<G4VEmModel*,G4VEmModel*> (newModel,newModel2);
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::ReplaceG4Model(G4VProcess* process, G4VEmModel* newModel, G4VEmModel* newModel2 )
{

  G4VEnergyLossProcess* processEL = dynamic_cast<G4VEnergyLossProcess*>(process);
  
  if ( processEL ) {
    // Delete old model and add new
    //t    std::vector<G4VEmModel*> models = processEL->GetModelManager()->GetModels();
    std::vector<G4VEmModel*> models = processEL->GetEmModels();
#ifndef GAMOS_NO_VERBOSE
	    if( PhysicsVerb(debugVerb) ) {
	      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
		G4cout << ii2 << "OLD GAMMA MODELS " << models[ii2]->GetName() << G4endl; 
	      }
	    }
#endif
    for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
      delete models[ii2];
    }
    processEL->modelManager->models.clear();
    processEL->modelManager->flucModels.clear();
    processEL->modelManager->regions.clear();
    processEL->modelManager->orderOfModels.clear();
    processEL->modelManager->isUsed.clear();
    processEL->modelManager->nEmModels = 0;
    processEL->AddEmModel(0,newModel);
    if( newModel2 ) processEL->AddEmModel(0,newModel2);
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) {
      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	G4cout << ii2 << "NEW GAMMA MODELS " << models[ii2]->GetName() << G4endl; 
      }
    }
#endif

  } else {
    G4VMultipleScattering* processMS = dynamic_cast<G4VMultipleScattering*>(process);
    if ( processMS ) {
      // Delete old model and add new
      std::vector<G4VMscModel*> models = processMS->GetMscModels(); 
      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	delete models[ii2];
      }
      processMS->modelManager->models.clear();
      processMS->modelManager->flucModels.clear();
      processMS->modelManager->regions.clear();
      processMS->modelManager->orderOfModels.clear();
      processMS->modelManager->isUsed.clear();
      processMS->modelManager->nEmModels = 0;
      G4VMscModel* newModelMS = dynamic_cast<G4VMscModel*>(newModel);
      G4VMscModel* newModel2MS = dynamic_cast<G4VMscModel*>(newModel2);
      processMS->AddEmModel(0,newModelMS);
      if( newModel2 ) processMS->AddEmModel(0,newModel2MS);
    } else {
      G4VEmProcess* processEM = dynamic_cast<G4VEmProcess*>(process); // G4CoulombScattering
      if ( ! processEM ) {
	G4Exception("void GmExtraPhysicsMessenger::ReplaceG4Model",
		    "Wrong model given",
		    FatalException,
		    G4String("PROCESSEM " + process->GetProcessName() + " it is not G4VEnergyLossProcess nor G4VMultipleScattering, please contact GAMOS authors").c_str());
      }
      // Delete old model and add new
      std::vector<G4VEmModel*> models = processEM->GetEmModels();
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) {
	for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	  G4cout << ii2 << "OLD CS MODELS " << models[ii2]->GetName() << G4endl; 
	}
      }
#endif
      for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	delete models[ii2];
      }
      processEM->modelManager->models.clear();
      processEM->modelManager->flucModels.clear();
      processEM->modelManager->regions.clear();
      processEM->modelManager->orderOfModels.clear();
      processEM->modelManager->isUsed.clear();
      processEM->modelManager->nEmModels = 0;
      processEM->AddEmModel(0,newModel);
      if( newModel2 ) processEM->AddEmModel(0,newModel2);

#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) {
	for( size_t ii2 = 0; ii2 < models.size(); ii2++) {
	  G4cout << ii2 << "NEW CS MODELS " << models[ii2]->GetName() << G4endl;
	}
      }
#endif

    }
  }
  
}
