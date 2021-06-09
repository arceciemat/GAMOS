///////////////////////////////////////////////////////////////////////////////
// File: GmAnalysisMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmAnalysisMessenger_h
#define GmAnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GmUIcmdWithAString.hh"

class GmAnalysisMessenger: public G4UImessenger {
public:
  GmAnalysisMessenger();
  ~GmAnalysisMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString*  setFormatCmd;
  GmUIcmdWithAString*  addFormatCmd;
  GmUIcmdWithAString*  histo1NBinsCmd;
  GmUIcmdWithAString*  histo1MinCmd;
  GmUIcmdWithAString*  histo1MaxCmd;
  GmUIcmdWithAString*  histo2NBinsXCmd;
  GmUIcmdWithAString*  histo2MinXCmd;
  GmUIcmdWithAString*  histo2MaxXCmd;
  GmUIcmdWithAString*  histo2NBinsYCmd;
  GmUIcmdWithAString*  histo2MinYCmd;
  GmUIcmdWithAString*  histo2MaxYCmd;
  GmUIcmdWithAString*  histoPrintCmd;

};

#endif
