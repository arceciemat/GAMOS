#ifndef __GmKillAllUA__
#define __GmKillAllUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"


class GmKillAllUA :  public GmUserStackingAction, 
		     public GmUserSteppingAction
{
public:
  GmKillAllUA();
  ~GmKillAllUA(){};
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );

  virtual void UserSteppingAction( const G4Step* );

};

#endif
