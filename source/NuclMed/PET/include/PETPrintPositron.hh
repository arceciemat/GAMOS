#ifndef __PETPrintPositron__
#define __PETPrintPositron__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class G4Event;

class PETPrintPositron : public GmUserSteppingAction
{
public:
  PETPrintPositron();
  ~PETPrintPositron(){};
  virtual void UserSteppingAction(const G4Step* aStep);

private:

};

#endif
