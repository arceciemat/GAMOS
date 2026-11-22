#ifndef GmG4HadronicPhysicsListsMessenger_h
#define GmG4HadronicPhysicsListsMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GmG4HadronicPhysicsLists;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmG4HadronicPhysicsListsMessenger: public G4UImessenger
{
public:
  
  GmG4HadronicPhysicsListsMessenger(GmG4HadronicPhysicsLists* );
  ~GmG4HadronicPhysicsListsMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  
  GmG4HadronicPhysicsLists* pPhysicsList;
  
  G4UIdirectory*             physDir;    
  G4UIcmdWithAString*        pListCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

