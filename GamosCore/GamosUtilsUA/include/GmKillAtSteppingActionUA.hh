#ifndef __GmKillAtSteppingActionUA__
#define __GmKillAtSteppingActionUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmKillAtSteppingActionUA :  public GmUserSteppingAction
{
public:
  GmKillAtSteppingActionUA();
  ~GmKillAtSteppingActionUA(){};

  virtual void UserSteppingAction( const G4Step* );

};

#endif
