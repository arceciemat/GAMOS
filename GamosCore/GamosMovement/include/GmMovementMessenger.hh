///////////////////////////////////////////////////////////////////////////////
// File: GmMovementMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmMovementMessenger_h
#define GmMovementMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmGenerator;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

class GmMovementMessenger: public G4UImessenger {
public:
  GmMovementMessenger();
  ~GmMovementMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmGenerator*   myAction;
  
  GmUIcmdWithAString* MoveEachNEventsCmd;
  GmUIcmdWithAString* MoveEachTimeCmd;
  GmUIcmdWithAString* MoveFromFileCmd;
};

#endif
