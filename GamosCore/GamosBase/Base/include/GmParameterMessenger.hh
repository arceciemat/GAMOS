///////////////////////////////////////////////////////////////////////////////
// File: GmParameterMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmParameterMessenger_h
#define GmParameterMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmParameterMgr;

#include "GmUIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

class GmParameterMessenger: public G4UImessenger {
public:
  GmParameterMessenger(GmParameterMgr*);
  ~GmParameterMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmParameterMgr*   myAction;
  
  GmUIcmdWithAString*        AddParamCmd;
  G4UIcmdWithAnInteger*      PrintParamCmd;

};

#endif
