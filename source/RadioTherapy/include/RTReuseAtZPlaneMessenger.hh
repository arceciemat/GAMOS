#ifndef RTReuseAtZPlaneMessenger_h
#define RTReuseAtZPlaneMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RTReuseAtZPlaneMessenger: public G4UImessenger
{
public:
  RTReuseAtZPlaneMessenger();
  ~RTReuseAtZPlaneMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  G4UIdirectory*  theRTDir;
  G4UIcmdWithoutParameter* theReuseCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
