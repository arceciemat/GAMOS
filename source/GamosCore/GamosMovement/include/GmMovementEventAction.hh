#ifndef __GmMovementEventAction__
#define __GmMovementEventAction__

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <iostream>

class GmGenerator;


class GmMovementEventAction : public GmUserEventAction, public GmUserRunAction
{
public:
  GmMovementEventAction();
  ~GmMovementEventAction(){};
  virtual void BeginOfEventAction(const G4Event* anEvent);
  virtual void BeginOfRunAction(const G4Run*);
 
private:

  GmGenerator* theGmGenerator;
};

#endif
