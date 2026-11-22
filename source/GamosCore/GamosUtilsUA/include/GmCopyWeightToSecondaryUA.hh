#ifndef GmCopyWeightToSecondaryUA_hh
#define GmCopyWeightToSecondaryUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <set>

class GmCopyWeightToSecondaryUA : public GmUserSteppingAction
{
public:
  GmCopyWeightToSecondaryUA();
  ~GmCopyWeightToSecondaryUA(){};
  virtual void UserSteppingAction( const G4Step* aStep );

private:
};

#endif
