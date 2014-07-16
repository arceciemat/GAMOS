#ifndef GmBaseMessenger_HH
#define GmBaseMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

class GmBaseMessenger: public G4UImessenger {
public:
  GmBaseMessenger();
  ~GmBaseMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString* filterCmd;
  GmUIcmdWithAString* classifierCmd;
  GmUIcmdWithAString* classifierIndicesCmd;
  GmUIcmdWithAString* distributionCmd;

};

#endif
