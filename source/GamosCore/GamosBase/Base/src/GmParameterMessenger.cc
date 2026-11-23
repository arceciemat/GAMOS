#include "GmParameterMessenger.hh"
#include "GmParameterMgr.hh"

#include "G4UImanager.hh"
#include "GmUIcmdWithAString.hh"

GmParameterMessenger::GmParameterMessenger(GmParameterMgr* myua): 
  myAction(myua) 
{
  AddParamCmd = new GmUIcmdWithAString("/gamos/setParam",this);
  AddParamCmd->SetGuidance("Set Parameter value(s)");
  AddParamCmd->SetParameterName("choice",true);
  AddParamCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  AddParamCmd2 = new GmUIcmdWithAString("/P",this);
  AddParamCmd2->SetGuidance("Set Parameter value(s)");
  AddParamCmd2->SetParameterName("choice",true);
  AddParamCmd2->AvailableForStates(G4State_PreInit,G4State_Idle);

  PrintParamCmd = new G4UIcmdWithAnInteger("/gamos/base/printParametersUsage",this);
  PrintParamCmd->SetGuidance("Print the usage of Parameters");
  PrintParamCmd->SetParameterName("choice",true);
  PrintParamCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

GmParameterMessenger::~GmParameterMessenger() 
{
  if (AddParamCmd) delete AddParamCmd;
  if (AddParamCmd2) delete AddParamCmd2;
  if (PrintParamCmd) delete PrintParamCmd;
}

void GmParameterMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == AddParamCmd || 
      command == AddParamCmd2) {
    myAction->AddParam(newValues);
  }
  else if (command == PrintParamCmd) {
    myAction->PrintParametersUsage(PrintParamCmd->GetNewIntValue(newValues));
  }
  return;
}
