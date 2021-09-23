#include "RTPlanMessenger.hh"
#include "RTVPlanSource.hh"
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
RTPlanMessenger::RTPlanMessenger(RTVPlanSource* myua) 
   : myAction(myua) 
{
  AssociatePlanVolumesCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/associatePlanVolumes",this);
  AssociatePlanVolumesCmd->SetParameterName("PARTICLE_SOURCE NAME_AT_RTPlan NAME_AT_GEOM",false);
  AssociatePlanVolumesCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  DefineAcceleratorVolumeCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/defineAcceleratorVolume",this);
  DefineAcceleratorVolumeCmd->SetParameterName("PARTICLE_SOURCE VOLUME_NAME",false);
  DefineAcceleratorVolumeCmd->AvailableForStates(G4State_Idle,G4State_PreInit);
}

//------------------------------------------------------------------------
RTPlanMessenger::~RTPlanMessenger() 
{
  if (AssociatePlanVolumesCmd) delete AssociatePlanVolumesCmd;
  if (DefineAcceleratorVolumeCmd) delete DefineAcceleratorVolumeCmd;
}


//------------------------------------------------------------------------
void RTPlanMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  if (command == AssociatePlanVolumesCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->AssociatePlanVolumes(wl);
  } else if (command == DefineAcceleratorVolumeCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->DefineAcceleratorVolume(wl);
  }

  return;
}

