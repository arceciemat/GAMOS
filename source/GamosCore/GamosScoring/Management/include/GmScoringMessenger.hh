#ifndef GmScoringMessenger_HH
#define GmScoringMessenger_HH

#include "globals.hh"
#include "G4UImessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

class GmScoringMgr;

class GmScoringMessenger: public G4UImessenger {
public:
  GmScoringMessenger();
  ~GmScoringMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString* PrinterCmd;
  GmUIcmdWithAString* PrinterCmd2;
  GmUIcmdWithAString* MFDetectorCmd;
  GmUIcmdWithAString* Scorer2MFDCmd;
  GmUIcmdWithAString* ScorerToMFDCmd;
  GmUIcmdWithAString* ScorerCmd;
  GmUIcmdWithAString* Filter2ScorerCmd;
  GmUIcmdWithAString* FilterToScorerCmd;
  GmUIcmdWithAString* Printer2ScorerCmd;
  GmUIcmdWithAString* PrinterToScorerCmd;
  GmUIcmdWithAString* Classifier2ScorerCmd;
  GmUIcmdWithAString* ClassifierToScorerCmd;
  GmUIcmdWithAString* TrkWeightCmd;
  GmUIcmdWithAString* ScoreErrorsCmd;
  GmUIcmdWithAString* PrintByEventCmd;
  GmUIcmdWithAString* PrintNEventsTypeCmd;
  GmUIcmdWithAString* UnitCmd;

  GmScoringMgr* theAction;
};

#endif
