///////////////////////////////////////////////////////////////////////////////
// File: GmFactoriesMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmFactoriesMessenger_h
#define GmFactoriesMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmRunManager;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

class GmFactoriesMessenger: public G4UImessenger {
public:
  GmFactoriesMessenger(GmRunManager*);
  ~GmFactoriesMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmRunManager*   myAction;
  
  GmUIcmdWithAString*        geometryFCmd;
  GmUIcmdWithAString*        physicsListFCmd;
  GmUIcmdWithAString*        generatorFCmd;
  GmUIcmdWithAString*        userActionsFCmd;
  GmUIcmdWithAString*        verbosityFCmd;
  GmUIcmdWithAString*        digitizerFCmd;
  GmUIcmdWithAString*        rechitBuilderFCmd;

};

#endif
