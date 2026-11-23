#ifndef GmUserLimitsMessenger_HH
#define GmUserLimitsMessenger_HH

#include "globals.hh"
#include "G4UImessenger.hh"
class GmUIcmdWithAString;
class G4UIcmdWithoutParameter;

class GmUserLimitsMgr;

class GmUserLimitsMessenger: public G4UImessenger {
public:
  GmUserLimitsMessenger();
  ~GmUserLimitsMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString* regionCmd;
  GmUIcmdWithAString* prodCutsCmd;

  GmUIcmdWithAString* userLimitsCmd;
  GmUIcmdWithAString* maxStepCmd;
  GmUIcmdWithAString* maxTrkLenCmd;
  GmUIcmdWithAString* maxTOFCmd;
  GmUIcmdWithAString* minEKinCmd;
  GmUIcmdWithAString* minEKinByRangeCmd;
  GmUIcmdWithAString* minRangeCmd;

  GmUIcmdWithAString* addLVParticleCmd;
  
  G4UIcmdWithoutParameter* printCmd;

  GmUIcmdWithAString* prodCutsLimitsCmd;

  GmUserLimitsMgr* theAction;
};

#endif
