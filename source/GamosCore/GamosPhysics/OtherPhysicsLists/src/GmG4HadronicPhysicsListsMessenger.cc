#include "GmG4HadronicPhysicsListsMessenger.hh"

#include "GmG4HadronicPhysicsLists.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmG4HadronicPhysicsListsMessenger::GmG4HadronicPhysicsListsMessenger(GmG4HadronicPhysicsLists* pPhys)
:pPhysicsList(pPhys)
{
  physDir = new G4UIdirectory("/gamos/GmG4HadronicPhysics/");
  physDir->SetGuidance("physics list commands");

  pListCmd = new G4UIcmdWithAString("/gamos/GmG4HadronicPhysics/replacePhysics",this);  
  pListCmd->SetGuidance("Add modula physics list.");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmG4HadronicPhysicsListsMessenger::~GmG4HadronicPhysicsListsMessenger()
{
  delete pListCmd;
  delete physDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmG4HadronicPhysicsListsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == pListCmd )
   { 
     pPhysicsList->ReplacePhysicsList(newValue);
   }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
