#include "GmEmDNAPhysicsMessenger.hh"
#include "GmEmDNAPhysics.hh"
#include "G4UIdirectory.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

// ----------------------------------------------------------------------------
GmEmDNAPhysicsMessenger::GmEmDNAPhysicsMessenger(GmEmDNAPhysics * physList)
:theGmPhysicsList(physList)
{  
 listDir = new G4UIdirectory("/gamos/GmPhysics/");

 theGmPhysicsListCmd = new GmUIcmdWithAString("/gamos/GmPhysics/replacePhysics",this);  
 theGmPhysicsListCmd->SetGuidance("Replace a set of process models of a particle");
 theGmPhysicsListCmd->SetParameterName("physList",false);
 theGmPhysicsListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 theGmPhysicsListCmdOld = new GmUIcmdWithAString("/gamos/GmPhysics/addPhysics",this);  
 theGmPhysicsListCmdOld->SetGuidance("Replace a set of process models of a particle");
 theGmPhysicsListCmdOld->SetParameterName("physList",false);
 theGmPhysicsListCmdOld->AvailableForStates(G4State_PreInit,G4State_Idle);

}

// ----------------------------------------------------------------------------
GmEmDNAPhysicsMessenger::~GmEmDNAPhysicsMessenger()
{
  delete theGmPhysicsListCmd;
  delete theGmPhysicsListCmdOld;
  delete listDir;
}

// ----------------------------------------------------------------------------
void GmEmDNAPhysicsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == theGmPhysicsListCmd){
    G4bool bFound = theGmPhysicsList->ReplacePhysicsList(newValue);
    if( !bFound ) {
      G4Exception("GmEmDNAPhysicsMessenger::SetNewValue","ERROR in /gamos/GmPhysics/replacePhysics command, process model set",FatalErrorInArgument,newValue);
    }
  }else if (command == theGmPhysicsListCmdOld){
    G4Exception("GmEmDNAPhysicsMessenger::SetNewValue",
		"WARNING",
		JustWarning,
		"You are using a deprecated command /gamos/GmPhysics/addPhysics , please use /gamos/GmPhysics/replacePhysics");
    G4bool bFound = theGmPhysicsList->ReplacePhysicsList(newValue);
    if( !bFound ) {
      G4Exception("GmEmDNAPhysicsMessenger::SetNewValue","ERROR in /gamos/GmPhysics/replacePhysics command, process model set",FatalErrorInArgument,newValue);
    }
  } 
}
