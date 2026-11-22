#ifndef GmCopyWeightToRDSecondaryUA_hh
#define GmCopyWeightToRDSecondaryUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <set>

class GmCopyWeightToRDSecondaryUA : public GmUserSteppingAction
{
public:
  GmCopyWeightToRDSecondaryUA();
  ~GmCopyWeightToRDSecondaryUA(){};
  virtual void UserSteppingAction( const G4Step* aStep );

private:
};

#endif
