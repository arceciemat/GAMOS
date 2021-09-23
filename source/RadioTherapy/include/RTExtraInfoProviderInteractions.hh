#ifndef __RTExtraInfoProviderInteractions__
#define __RTExtraInfoProviderInteractions__

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "RTVExtraInfoProviderLongByRegion.hh"

class RTExtraInfoProviderInteractions : public GmUserTrackingAction, public GmUserSteppingAction, public RTVExtraInfoProviderLongByRegion
{
public:
  RTExtraInfoProviderInteractions();
  ~RTExtraInfoProviderInteractions(){}; 
  virtual void PreUserTrackingAction( const G4Track* aTrack );
  virtual void UserSteppingAction( const G4Step* aStep );

private:
};

#endif
