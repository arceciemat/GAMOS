///////////////////////////////////////////////////////////////////////////////
// File: RTPhaseSpaceMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef RTPhaseSpaceMessenger_h
#define RTPhaseSpaceMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GmUIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

class RTPhaseSpaceMessenger: public G4UImessenger {
public:
  RTPhaseSpaceMessenger();
  ~RTPhaseSpaceMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:

  G4bool AssociateSD2Vol( G4String& sdclass, G4String& sdtype, G4String& lvname );

private:
  GmUIcmdWithAString*        assocCmd;

  GmUIcmdWithAString* nSimpleNAncesCmd;
  GmUIcmdWithAString* nSimpleNShiftCmd;

};

#endif
