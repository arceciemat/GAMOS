///////////////////////////////////////////////////////////////////////////////
// File: GmSDMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmSDMessenger_h
#define GmSDMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

class GmSDMessenger: public G4UImessenger {
public:
  GmSDMessenger();
  ~GmSDMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:

  void AssociateSD2LogVol( G4String& sdclass, G4String& sdtype, G4String& lvname );

private:
  GmUIcmdWithAString*        assocCmd;

  GmUIcmdWithAString* nSimpleNAncesCmd;
  GmUIcmdWithAString* nSimpleNShiftCmd;

};

#endif
