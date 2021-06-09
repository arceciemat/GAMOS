#ifndef __GmKillAtStackingActionUA__
#define __GmKillAtStackingActionUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"

class GmKillAtStackingActionUA :  public GmUserStackingAction
{
public:
  GmKillAtStackingActionUA();
  ~GmKillAtStackingActionUA(){};
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );

};

#endif
