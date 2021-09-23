///////////////////////////////////////////////////////////////////////////////
// File : GmKillTooManyStepsUA.h
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmKillTooManyStepsUA_h
#define GmKillTooManyStepsUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include "globals.hh"

#include <map>
#include <set>
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmKillTooManyStepsUA : public GmUserRunAction, 
			     public GmUserSteppingAction
{

 public:
  GmKillTooManyStepsUA();
  ~GmKillTooManyStepsUA();

 public:
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction(const G4Step* aStep);

 private:
  G4int theNStepsMax;
  G4int theNStepsWarning;

};

#endif
