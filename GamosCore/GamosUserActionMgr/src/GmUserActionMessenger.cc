#include "GmUserActionMessenger.hh"
#include "GmUserActionMgr.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

GmUserActionMessenger::GmUserActionMessenger(GmUserActionMgr* myua): 
  myAction(myua) 
{
  listCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpList",this);
  listCmd->SetGuidance("Dump the list of UserActions saying if they are enabled or Disabled");
  listCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  listONCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpListON",this);
  listONCmd->SetGuidance("Dump the list of ENABLED UserActions");
  listONCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  listOFFCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpListOFF",this);
  listOFFCmd->SetGuidance("Dump the list of DISABLED UserActions");
  listOFFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EnableCmd = new GmUIcmdWithAString("/gamos/userAction/Enable",this);
  EnableCmd->SetGuidance("Enable a User Action by name");
  EnableCmd->SetParameterName("choice",true);
  EnableCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  DisableCmd = new GmUIcmdWithAString("/gamos/userAction/Disable",this);
  DisableCmd->SetGuidance("Disable a User Action by name");
  DisableCmd->SetParameterName("choice",true);
  DisableCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

GmUserActionMessenger::~GmUserActionMessenger() 
{
  if (listCmd) delete listCmd;
  if (listONCmd) delete listONCmd;
  if (listOFFCmd) delete listOFFCmd;
  if (EnableCmd) delete EnableCmd;
  if (DisableCmd) delete DisableCmd;
}

void GmUserActionMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == listCmd) {
    myAction->DumpUserActionList( G4cout, 0 );
  }else if (command == listONCmd) {
    myAction->DumpUserActionList( G4cout, 1 );
  }else if (command == listOFFCmd) {
    myAction->DumpUserActionList( G4cout, -1 );
  }else if (command == EnableCmd) {
    myAction->EnableUserAction(newValues);
  }else if (command == DisableCmd) {
    myAction->DisableUserAction(newValues);
  }
  return;
}
