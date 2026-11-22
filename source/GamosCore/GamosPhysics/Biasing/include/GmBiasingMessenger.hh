#ifndef GmBiasingMessenger_HH
#define GmBiasingMessenger_HH

#include "G4UImessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
class GmBiasingMgr;

class GmBiasingMessenger: public G4UImessenger {
public:
  GmBiasingMessenger();
  ~GmBiasingMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:

private:
  GmUIcmdWithAString* operatorCmd;
  GmUIcmdWithAString* oper2LogVolCmd;
  GmUIcmdWithAString* addProcCmd;
  GmUIcmdWithAString* addPartCmd;
  GmUIcmdWithAString* addPartProcCmd;

  GmBiasingMgr* theManager;
};

#endif
