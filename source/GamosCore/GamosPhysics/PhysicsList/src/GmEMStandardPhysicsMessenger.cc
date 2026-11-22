#include "GmEMStandardPhysicsMessenger.hh"

#include "GmEMStandardPhysics.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEMStandardPhysicsMessenger::GmEMStandardPhysicsMessenger(GmEMStandardPhysics* pPhys)
:pPhysicsList(pPhys)
{
  physDir = new G4UIdirectory("/gamos/EMStandardPhysics/");
  physDir->SetGuidance("physics list commands");

  pListCmd = new G4UIcmdWithAString("/gamos/EMStandardPhysics/replacePhysics",this);  
  pListCmd->SetGuidance("Add modula physics list.");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEMStandardPhysicsMessenger::~GmEMStandardPhysicsMessenger()
{
  delete pListCmd;
  delete physDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEMStandardPhysicsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == pListCmd )
   { 
     pPhysicsList->ReplacePhysicsList(newValue);
   }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
