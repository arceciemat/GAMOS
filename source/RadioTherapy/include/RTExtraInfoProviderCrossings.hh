#ifndef __RTExtraInfoProviderCrossings__
#define __RTExtraInfoProviderCrossings__

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "RTVExtraInfoProviderLongByRegion.hh"

class RTExtraInfoProviderCrossings : public GmUserTrackingAction, public GmUserSteppingAction, public RTVExtraInfoProviderLongByRegion
{
public:
  RTExtraInfoProviderCrossings();
  ~RTExtraInfoProviderCrossings(){}; 
  virtual void PreUserTrackingAction( const G4Track* aTrack );
  virtual void UserSteppingAction( const G4Step* aStep );

private:
};

#endif
