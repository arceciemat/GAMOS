#ifndef __GmKillAtSteppingActionRandomUA__
#define __GmKillAtSteppingActionRandomUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmKillAtSteppingActionRandomUA :  public GmUserSteppingAction,  public GmUserRunAction
{
public:
  GmKillAtSteppingActionRandomUA();
  ~GmKillAtSteppingActionRandomUA(){};
  void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* );

private:
  G4double theProbability;
};

#endif
