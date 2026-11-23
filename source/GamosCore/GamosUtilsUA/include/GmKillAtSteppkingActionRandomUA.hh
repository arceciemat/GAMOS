#ifndef __GmKillAtSteppkingActionRandomUA__
#define __GmKillAtSteppkingActionRandomUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmKillAtSteppkingActionRandomUA :  public GmUserSteppingAction,  public GmUserRunAction
{
public:
  GmKillAtSteppkingActionRandomUA();
  ~GmKillAtSteppkingActionRandomUA(){};
  void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* );

private:
  G4double theProbability;
};

#endif
