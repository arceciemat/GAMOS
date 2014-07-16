#include "GmGeneratorMessenger.hh"
#include "GmGenerator.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4NucleiProperties.hh"
#include "G4ProcessManager.hh"
#include "GmParticleSource.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ProcessManager.hh"

//------------------------------------------------------------------------
GmGeneratorMessenger::GmGeneratorMessenger(GmGenerator* myua) 
   : myAction(myua) 
{
  AddSingleParticleCmd = new GmUIcmdWithAString("/gamos/generator/addSingleParticleSource",this);
  AddSingleParticleCmd->SetGuidance("Add single particle source to list of active particle sources");
  AddSingleParticleCmd->SetParameterName("NAME PARTICLE_NAME ENERGY",false);
  AddSingleParticleCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  AddIsotopeCmd = new GmUIcmdWithAString("/gamos/generator/addIsotopeSource",this);
  AddIsotopeCmd->SetGuidance("Add isotope to list of active particle sources");
  AddIsotopeCmd->SetParameterName("NAME ACTIVITY",false);
  AddIsotopeCmd->AvailableForStates(G4State_Idle,G4State_PreInit);
  
  AddDoubleBackToBackParticleCmd = new GmUIcmdWithAString("/gamos/generator/addDoubleBackToBackParticleSource",this);
  AddDoubleBackToBackParticleCmd->SetGuidance("Add particle source of two particles with opposite directions to list of active particle sources");
  AddDoubleBackToBackParticleCmd->SetParameterName("NAME PARTICLE_NAME ENERGY",false);
  AddDoubleBackToBackParticleCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  DistTimeCmd = new GmUIcmdWithAString("/gamos/generator/timeDist",this);
  DistTimeCmd->SetGuidance("Sets the time distribution type for one source");
  DistTimeCmd->SetParameterName("SOURCE_NAME DISTRIBUTION_TYPE (EXTRA_PARAMETERS)",false);
  DistTimeCmd->AvailableForStates(G4State_Idle);

  DistEnergyCmd = new GmUIcmdWithAString("/gamos/generator/energyDist",this);
  DistEnergyCmd->SetGuidance("Sets the energy distribution type for one source");
  DistEnergyCmd->SetParameterName("SOURCE_NAME DISTRIBUTION_TYPE (EXTRA_PARAMETERS)",false);
  DistEnergyCmd->AvailableForStates(G4State_Idle);

  DistPositionCmd = new GmUIcmdWithAString("/gamos/generator/positionDist",this);
  DistPositionCmd->SetGuidance("Sets the position distribution type for one source");
  DistPositionCmd->SetParameterName("SOURCE_NAME DISTRIBUTION_TYPE (EXTRA_PARAMETERS)",false);
  DistPositionCmd->AvailableForStates(G4State_Idle);

  DistDirectionCmd = new GmUIcmdWithAString("/gamos/generator/directionDist",this);
  DistDirectionCmd->SetGuidance("Sets the direction distribution type for one source");
  DistDirectionCmd->SetParameterName("SOURCE_NAME DISTRIBUTION_TYPE (EXTRA_PARAMETERS)",false);
  DistDirectionCmd->AvailableForStates(G4State_Idle);

  PositionAddVolumeCmd = new GmUIcmdWithAString("/gamos/generator/GmPositionVolumesAndSurfaces/addVolumeOrSurface",this);
  PositionAddVolumeCmd->SetGuidance("Add a new volume to the position distribution of type GmPositionUserVolumes");
  PositionAddVolumeCmd->SetParameterName("NAME SOLID_TYPE POS_X POS_Y POS_Z ANG_X ANG_Y ANG_Z SOLID_PARAM_1 (SOLID_PARAM_2 ...)",false);
  PositionAddVolumeCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  DistributionCmd = new GmUIcmdWithAString("/gamos/generator/addBiasDistribution",this);
  DistributionCmd->SetGuidance("Add a new distribution to bias a variable");
  DistributionCmd->SetParameterName("GENERATOR_DIST_TYPE BIAS_DIST_NAME",false);
  DistributionCmd->AvailableForStates(G4State_Idle);
}

//------------------------------------------------------------------------
GmGeneratorMessenger::~GmGeneratorMessenger() 
{
  if (AddSingleParticleCmd) delete AddSingleParticleCmd;
  if (AddIsotopeCmd) delete AddIsotopeCmd;
  if (AddDoubleBackToBackParticleCmd) delete AddDoubleBackToBackParticleCmd;
  if (DistTimeCmd) delete DistTimeCmd;
  if (DistEnergyCmd) delete DistEnergyCmd;
  if (DistPositionCmd) delete DistPositionCmd;
  if (DistDirectionCmd) delete DistDirectionCmd;
  if (DirectionCmd) delete DirectionCmd;
  if (PositionCmd) delete PositionCmd;
  if (EnergyCmd) delete EnergyCmd;
  if (ActivityCmd) delete ActivityCmd;
  if (PositionAddVolumeCmd) delete PositionAddVolumeCmd;
  if (DistributionCmd) delete DistributionCmd;
}


//------------------------------------------------------------------------
void GmGeneratorMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == AddSingleParticleCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->AddSingleParticleSource(newValues);
  }else if (command == AddIsotopeCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: source_name isotope_name activity activity_unit"); 
    myAction->AddIsotopeSource(newValues);
  }else if (command == AddDoubleBackToBackParticleCmd) {
    myAction->AddDoubleBackToBackParticleSource(newValues);
  } else if (command == DistTimeCmd || command == DistEnergyCmd || command == DistPositionCmd || command == DistDirectionCmd ) {
    std::vector<G4String> wordlist = GmGenUtils::GetWordsInString( newValues );
    if( wordlist.size() < 2 ) {
      G4Exception(" GmGeneratorMessenger::SetNewValue",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: SOURCE_NAME DISTRIBUTION_TYPE (EXTRA_PARAMETERS)").c_str()); 
    }
    if (command == DistTimeCmd) {
      myAction->SetDistribution("time", wordlist );
    } else if (command == DistEnergyCmd) {
      myAction->SetDistribution("energy", wordlist );
    } else if (command == DistPositionCmd) {
      myAction->SetDistribution("position", wordlist );
    } else if (command == DistDirectionCmd) {
      myAction->SetDistribution("direction", wordlist );
    }
  } else if (command == PositionAddVolumeCmd ){
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    GmVGenerDistPosition* distPos = myAction->FindSource( wl[0] )->GetPositionDistribution();
    if(  distPos->GetName() != "GmVGenerDistPositionVolumesAndSurfaces" ) {
      G4Exception(" GmGeneratorMessenger::SetNewValue"
		  ,"ERROR in command 'GmPositionVolumesAndSurfaces/addVolumeOrSurface'"
		  ,FatalErrorInArgument
		  ,("source is not of type 'GmVGenerDistPositionVolumesAndSurfaces', but "+distPos->GetName()).c_str());
    }
    distPos->SetParams(wl);

  } else if (command == DistributionCmd ){
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 3 ) {
      G4Exception(" GmGeneratorMessenger::SetNewValue"
		  ,"ERROR in command 'addBiasDistribution'"
		  ,FatalErrorInArgument
		  ,"There should be three arguments: SOURCE_NAME GENERATOR_DIST_TYPE BIAS_DIST_NAME");
    }
    myAction->AddBiasDistribution(wl[0],wl[1],wl[2]);

  }

  return;
}

