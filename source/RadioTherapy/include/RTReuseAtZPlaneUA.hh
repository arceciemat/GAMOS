#ifndef __RTReuseAtZPlaneUA__
#define __RTReuseAtZPlaneUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class RTReuseAtZPlaneUA : public GmUserRunAction, public GmUserSteppingAction
{
public:
  RTReuseAtZPlaneUA();
  ~RTReuseAtZPlaneUA();

  void UserSteppingAction(const G4Step* aStep);

private:

};

#endif
