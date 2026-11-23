#ifndef __GmKillAtStackingActionRandomUA__
#define __GmKillAtStackingActionRandomUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class GmKillAtStackingActionRandomUA :  public GmUserStackingAction,  public GmUserRunAction, public GmUserEventAction
{
public:
  GmKillAtStackingActionRandomUA();
  ~GmKillAtStackingActionRandomUA(){};
  void BeginOfRunAction(const G4Run* );
  void     BeginOfEventAction(const G4Event* );  
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );

private:
  G4double theProbability;
  G4bool bKillEvent; 
  G4bool   isEventKilled;
};

#endif
