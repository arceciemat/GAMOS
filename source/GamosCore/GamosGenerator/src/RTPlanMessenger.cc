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
  AssociatePlanVolumesCmd->SetParameterName("SOURCE_NAME RTPlanControlPoint_NAME GEOMETRY_NAME",false);
  AssociatePlanVolumesCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  DefineAcceleratorVolumeCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/defineAcceleratorVolume",this);
  DefineAcceleratorVolumeCmd->SetParameterName("NAME PARTICLE_NAME ENERGY",false);
  DefineAcceleratorVolumeCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  AssociatePlanLDACmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/associatePlanLimitingDeviceAngle",this);
  AssociatePlanLDACmd->SetParameterName("SOURCE_NAME GEOMETRY_NAME",false);
  AssociatePlanLDACmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  AssociateBeamVolumesCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/associateBeamVolumes",this);
  AssociateBeamVolumesCmd->SetParameterName("SOURCE_NAME RTPlanControlPoint_NAME GEOMETRY_NAME",false);
  AssociateBeamVolumesCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

}

//------------------------------------------------------------------------
RTPlanMessenger::~RTPlanMessenger() 
{
  if (AssociatePlanVolumesCmd) delete AssociatePlanVolumesCmd;
  if (DefineAcceleratorVolumeCmd) delete DefineAcceleratorVolumeCmd;
  if (AssociatePlanLDACmd) delete AssociatePlanLDACmd;
  if (AssociateBeamVolumesCmd) delete AssociateBeamVolumesCmd;
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
  } else if (command == AssociatePlanLDACmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->AssociatePlanLimitingDeviceAngle(wl);
  } else if (command == AssociateBeamVolumesCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->AssociateBeamVolumes(wl);
  }
  
}

