///////////////////////////////////////////////////////////////////////////////
// File: GmIsotopeMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmIsotopeMessenger_h
#define GmIsotopeMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmIsotopeMgr;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class GmIsotopeMessenger: public G4UImessenger {
public:
  GmIsotopeMessenger(GmIsotopeMgr*);
  ~GmIsotopeMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmIsotopeMgr*   myAction;
  
  GmUIcmdWithAString*  AddIsotopeCmd;
  G4UIcmdWithADoubleAndUnit* ActivityCmd;
};

#endif
