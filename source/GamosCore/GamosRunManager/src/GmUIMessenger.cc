#include "GmUIMessenger.hh"
#include "GmUIterminal.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmUIMessenger::GmUIMessenger(GmUIterminal* ses) : theCommand(ses)
{
  setCoutCmd = new GmUIcmdWithAString("/gamos/log/setCoutFile",this);
  setCoutCmd->SetGuidance("Select the file where the standard output will be copied ");
  setCoutCmd->SetParameterName("choice",true);
  setCoutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  setCerrCmd = new GmUIcmdWithAString("/gamos/log/setCerrFile",this);
  setCerrCmd->SetGuidance("Select the file where the standard error will be copied ");
  setCerrCmd->SetParameterName("choice",true);
  setCerrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  writeLogCmd = new G4UIcmdWithABool("/gamos/log/writeFiles",this);
  writeLogCmd->SetGuidance("Select if the log files are written or not ");
  writeLogCmd->SetParameterName("choice",true);
  writeLogCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmUIMessenger::~GmUIMessenger() 
{
  if (setCoutCmd) delete setCoutCmd;
  if (setCerrCmd) delete setCerrCmd;
  if (writeLogCmd) delete writeLogCmd;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void GmUIMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == setCoutCmd) {
    theCommand->SetCoutFile( newValues );
  }
  else if (command == setCerrCmd) {
    theCommand->SetCerrFile( newValues );
  }
  else if (command == writeLogCmd) {
    theCommand->SetWriteLog( writeLogCmd->GetNewBoolValue(newValues) );
  }

}
