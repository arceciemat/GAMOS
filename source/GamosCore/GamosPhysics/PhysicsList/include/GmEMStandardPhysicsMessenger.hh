#ifndef GmEMStandardPhysicsMessenger_h
#define GmEMStandardPhysicsMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GmEMStandardPhysics;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEMStandardPhysicsMessenger: public G4UImessenger
{
  public:
  
    GmEMStandardPhysicsMessenger(GmEMStandardPhysics* );
   ~GmEMStandardPhysicsMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    GmEMStandardPhysics* pPhysicsList;
    
    G4UIdirectory*             physDir;    
    G4UIcmdWithAString*        pListCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

