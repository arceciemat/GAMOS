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

#include "G4BetheHeitlerModel.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "GmLivermoreRayleighModel_XSChangeNEW.hh"
#include "G4PenelopeRayleighModel.hh"

#include "G4MollerBhabhaModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4PenelopeIonisationModel.hh"

#include "G4MollerBhabhaModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4PenelopeIonisationModel.hh"

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
  theAddPhysicsCmd->SetGuidance("Add new physics: decay, radioactiveDecay, opticalphoton, cerenkov, gamma-nuclear, electron-nuclear, positron-nuclear, xray-refraction, coulombScattering");
  theAddPhysicsCmd->AvailableForStates(G4State_Idle);

  // remove processes by type
  theRemoveProcessesByTypeCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByType",this);  
  theRemoveProcessesByTypeCmd->SetGuidance("Remove processes of given types: NotDefined, Transportation, Electromagnetic, Optical, Hadronic, Photolepton_hadron, Decay, General, Parameterisation, UserDefined");
  theRemoveProcessesByTypeCmd->AvailableForStates(G4State_Idle);
  
  // delete processes by name
  theRemoveProcessesByNameCmd = new GmUIcmdWithAString("/gamos/physics/removeProcessesByName",this);  
  theRemoveProcessesByNameCmd->SetGuidance("Remove processes of given name");
  theRemoveProcessesByNameCmd->AvailableForStates(G4State_Idle);
  
  // delete processes by particle and name
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
  delete theRemoveProcessesByTypeCmd;
  delete theRemoveProcessesByNameCmd;
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

  } else if (command == theRemoveProcessesByTypeCmd){
    RemoveProcessesByType(newValue);

  } else if (command == theRemoveProcessesByNameCmd){
    RemoveProcessesByName(newValue);

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

  } else if( newValue == "coulombScattering" ) {
    GmPhysicsCoulombScattering* coulScat = new GmPhysicsCoulombScattering();
    coulScat->ConstructProcess();

  } else {
    G4Exception("GmExtraPhysicsMessenger::AddPhysics",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("ARGUMENT is = " + newValue + " .It may be: decay, radioactiveDecay, opticalphoton, cerenkov, gamma-nuclear, electron-nuclear, positron-nuclear, xray-refraction").c_str());
  }
  
}

// ----------------------------------------------------------------------------
void GmExtraPhysicsMessenger::RemoveProcessesByType(G4String newValue)
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
void GmExtraPhysicsMessenger::RemoveProcessesByName(G4String newValue)
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
void GmExtraPhysicsMessenger::RemoveProcessesByParticleAndName(G4String newValue)
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
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator -> value();
    G4String particleName = particle -> GetParticleName();
    //      G4cout << " particleName " << particleName << G4endl;
    if (particleName == newParticleName ) {
      if( newParticleName == "gamma" ) {	  
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4VEmProcess* process = (G4VEmProcess*)((*procVector)[ii]);
	  G4String processName = process->GetProcessName();
	  G4VEmModel* newModel = 0;
	  if( processName == newProcessName ) {
	    if( processName == "phot" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4PEEffectFluoModel;
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermorePhotoElectricModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopePhotoElectricModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "compt" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4KleinNishinaCompton;
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreComptonModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeComptonModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "conv" ) { 
	      if( newModelName == "standard" ) {
		newModel = new G4BetheHeitlerModel;
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreGammaConversionModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeGammaConversionModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE" + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else if( processName == "Rayl" ) { 
	      if( newModelName == "standard" ) {	   
	      } else if( newModelName == "low-energy" ) {
		newModel = new GmLivermoreRayleighModel_XSChangeNEW();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeRayleighModel();
		newModel->SetHighEnergyLimit(1.*CLHEP::GeV);
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE" + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else {
	      continue;
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    // Delete old model and add new
	    std::vector<G4VEmModel*> models = process->modelManager->models;
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
	    
	  }
	}
      } else if( newParticleName == "e-" ) {
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4VEnergyLossProcess* process = (G4VEnergyLossProcess*)((*procVector)[ii]);
	  G4String processName = process->GetProcessName();
	  if( processName == newProcessName ) {
	    G4VEmModel* newModel = 0;
	    G4VEmModel* newModel2 = 0;
	    
	    if( processName == "eIoni" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4MollerBhabhaModel;
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreIonisationModel();
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeIonisationModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "eBrem" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4SeltzerBergerModel();
		newModel2 = new G4eBremsstrahlungRelModel();
		newModel->SetLowEnergyLimit(process->MinKinEnergy());
		newModel->SetHighEnergyLimit(newModel2->LowEnergyLimit());
		newModel2->SetHighEnergyLimit(process->MaxKinEnergy());                
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreBremsstrahlungModel();
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeBremsstrahlungModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE" + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "msc" ) {
	      if( newModelName == "Urban" ) {
		newModel = new G4UrbanMscModel();
	      }else if( newModelName == "WentzelVI" ) {
		newModel = new G4WentzelVIModel();
	      }else if( newModelName == "GoudsmitSaunderson" ) {
		newModel = new G4GoudsmitSaundersonMscModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    } else {
	      continue;
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    
	    // Delete old modle and add new
	    std::vector<G4VEmModel*> models = process->modelManager->models;
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
	    if( newModel2 ) process->AddEmModel(0,newModel2);
	  }
	}
      } else if( newParticleName == "e+" ) {
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4ProcessVector* procVector = pmanager->GetProcessList();
	for( size_t ii = 0; ii < procVector->size(); ii++ ) {
	  G4VEnergyLossProcess* process = (G4VEnergyLossProcess*)((*procVector)[ii]);
	  G4String processName = process->GetProcessName();
	  if( processName == newProcessName ) {
	    G4VEmModel* newModel = 0;
	    G4VEmModel* newModel2 = 0;
	    
	    if( processName == "eIoni" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4MollerBhabhaModel;
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreIonisationModel();
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeIonisationModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "eBrem" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4SeltzerBergerModel();
		newModel2 = new G4eBremsstrahlungRelModel();
		newModel->SetLowEnergyLimit(process->MinKinEnergy());
		newModel->SetHighEnergyLimit(newModel2->LowEnergyLimit());
		newModel2->SetHighEnergyLimit(process->MaxKinEnergy());                
	      } else if( newModelName == "low-energy" ) {
		newModel = new G4LivermoreBremsstrahlungModel();
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeBremsstrahlungModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "annihil" ) {
	      if( newModelName == "standard" ) {
		newModel = new G4eeToTwoGammaModel();
	      } else if( newModelName == "low-energy" ) {
	      } else if( newModelName == "penelope" ) {
		newModel = new G4PenelopeAnnihilationModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE" + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	      
	    } else if( processName == "msc" ) {
	      if( newModelName == "Urban" ) {
		newModel = new G4UrbanMscModel();
	      }else if( newModelName == "WentzelVI" ) {
		newModel = new G4WentzelVIModel();
	      }else if( newModelName == "GoudsmitSaunderson" ) {
		newModel = new G4GoudsmitSaundersonMscModel();
	      } else {
		G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
			    "Wrong argument given",
			    FatalException,
			    G4String("PARTICLE " + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
	      }
	    }
	    
	    if( newModel == 0 ) continue; // rayleigh for standard models
	    
	    // Delete old model and add new
	    std::vector<G4VEmModel*> models = process->modelManager->models;
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
	    if( newModel2 ) process->AddEmModel(0,newModel2);
	  }
	}
      } else {
	G4Exception("void GmExtraPhysicsMessenger::ReplaceModel",
		    "Wrong argument given",
		    FatalException,
		    G4String("PARTICLE" + newParticleName + " PROCESS " + newProcessName + " MODEL " + newModelName).c_str());
      }
      
    }  
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
    
