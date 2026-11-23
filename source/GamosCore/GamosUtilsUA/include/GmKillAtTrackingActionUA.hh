#ifndef GmKillAtTrackingActionUA_h
#define GmKillAtTrackingActionUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "G4TrackingManager.hh"

class GmKillAtTrackingActionUA : public GmUserTrackingAction
{
public:
  GmKillAtTrackingActionUA();
  ~GmKillAtTrackingActionUA(){};
  
 public:
  virtual void PreUserTrackingAction(const G4Track* aTrack);

};

#endif
