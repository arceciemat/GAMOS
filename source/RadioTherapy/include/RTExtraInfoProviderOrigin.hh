#ifndef __RTExtraInfoProviderOrigin__
#define __RTExtraInfoProviderOrigin__

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "RTVExtraInfoProviderLongByRegion.hh"

class RTExtraInfoProviderOrigin : public GmUserTrackingAction, public RTVExtraInfoProviderLongByRegion
{
public:
  RTExtraInfoProviderOrigin();
  ~RTExtraInfoProviderOrigin(){}; 
  virtual void PreUserTrackingAction( const G4Track* aTrack );

private:
};

#endif
