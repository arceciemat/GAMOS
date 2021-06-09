///////////////////////////////////////////////////////////////////////////////
// File: GmRunMessenger.h
// Date: 27/03/2002 P.Arce
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmRunMessenger_HH
#define GmRunMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
class GmRunManager;

class GmRunMessenger: public G4UImessenger {
public:
  GmRunMessenger(GmRunManager* runMgr);
  ~GmRunMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString* randomSetSeedsCmd;
  GmUIcmdWithAString* randomRestoreSeedsCmd;
  GmUIcmdWithAString* randomEngineCmd;
  GmRunManager* theRunManager;
};

#endif
