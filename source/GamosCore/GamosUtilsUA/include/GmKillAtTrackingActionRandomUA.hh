#ifndef __GmKillAtTrackingActionRandomUA__
#define __GmKillAtTrackingActionRandomUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmKillAtTrackingActionRandomUA :  public GmUserTrackingAction,  public GmUserRunAction
{
public:
  GmKillAtTrackingActionRandomUA();
  ~GmKillAtTrackingActionRandomUA(){};
  void BeginOfRunAction(const G4Run* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

private:
  G4double theProbability;
};

#endif
