///////////////////////////////////////////////////////////////////////////////
// File: GmUIMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmUIMessenger_h
#define GmUIMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmUIcmdWithAString;
class G4UIcmdWithABool;
class GmUIterminal;

class GmUIMessenger: public G4UImessenger {
public:
  GmUIMessenger(GmUIterminal* ses);
  ~GmUIMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString*  setCoutCmd;
  GmUIcmdWithAString*  setCerrCmd;

  G4UIcmdWithABool*  writeLogCmd;

  GmUIterminal* theCommand;
};

#endif
