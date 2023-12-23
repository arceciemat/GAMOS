#include "GmEmDNAPhysicsMessenger.hh"
#include "GmEmDNAPhysics.hh"
#include "G4UIdirectory.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

// ----------------------------------------------------------------------------
GmEmDNAPhysicsMessenger::GmEmDNAPhysicsMessenger(GmEmDNAPhysics * physList)
:theGmPhysicsList(physList)
{  
 listDir = new G4UIdirectory("/gamos/GmEmDNAPhysics/");

 theGmPhysicsListCmd = new GmUIcmdWithAString("/gamos/GmEmDNAPhysics/replacePhysics",this);  
 theGmPhysicsListCmd->SetGuidance("Replace a set of process models of a particle");
 theGmPhysicsListCmd->SetParameterName("physList",false);
 theGmPhysicsListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

// ----------------------------------------------------------------------------
GmEmDNAPhysicsMessenger::~GmEmDNAPhysicsMessenger()
{
  delete theGmPhysicsListCmd;
  delete listDir;
}

// ----------------------------------------------------------------------------
void GmEmDNAPhysicsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == theGmPhysicsListCmd){
    G4bool bFound = theGmPhysicsList->ReplacePhysicsList(newValue);
    if( !bFound ) {
      G4Exception("GmEmDNAPhysicsMessenger::SetNewValue","ERROR in /gamos/GmPhysics/replacePhysics command, invalid model ",FatalErrorInArgument,newValue);
    }
  } 
}
