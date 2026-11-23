#ifndef GmDeprecatedCommandsMessenger_HH
#define GmDeprecatedCommandsMessenger_HH 1

#include "G4UImessenger.hh"
class G4UIcommand;
#include <map>

class GmDeprecatedCommandsMessenger: public G4UImessenger {
public:
  GmDeprecatedCommandsMessenger();
  ~GmDeprecatedCommandsMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:

  std::map<G4UIcommand*,G4String> theCommands;
};

#endif
