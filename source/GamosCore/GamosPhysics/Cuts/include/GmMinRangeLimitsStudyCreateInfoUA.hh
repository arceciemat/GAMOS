#ifndef __GmMinRangeLimitsStudyCreateInfoUA__
#define __GmMinRangeLimitsStudyCreateInfoUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
class GmMinRangeLimitsStudyUA;
class GmCSTrackStepInfo;

class GmMinRangeLimitsStudyCreateInfoUA : public GmUserSteppingAction
{
public:
  GmMinRangeLimitsStudyCreateInfoUA(GmMinRangeLimitsStudyUA* mr);
  ~GmMinRangeLimitsStudyCreateInfoUA(){};
  virtual void UserSteppingAction( const G4Step* aStep );
  void SetInfoCreated( G4bool bif ){
    bInfoCreated = bif; }

private:
  GmMinRangeLimitsStudyUA* theMinRangeUA;
  G4bool bInfoCreated;

};

#endif
