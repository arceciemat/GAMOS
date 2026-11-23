///////////////////////////////////////////////////////////////////////////////
// File: GmUserActionMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmUserActionMessenger_h
#define GmUserActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmUserActionMgr;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

class GmUserActionMessenger: public G4UImessenger {
public:
  GmUserActionMessenger(GmUserActionMgr*);
  ~GmUserActionMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUserActionMgr*   myAction;
  
  G4UIcmdWithoutParameter*   listCmd;
  G4UIcmdWithoutParameter*   listONCmd;
  G4UIcmdWithoutParameter*   listOFFCmd;
  GmUIcmdWithAString*        EnableCmd;
  GmUIcmdWithAString*        DisableCmd;

};

#endif
