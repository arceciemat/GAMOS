#ifndef __RTExtraInfoProviderZOrigin__
#define __RTExtraInfoProviderZOrigin__

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "RTVExtraInfoProviderFloat.hh"

class RTExtraInfoProviderZOrigin : public GmUserTrackingAction, public RTVExtraInfoProviderFloat
{
public:
  RTExtraInfoProviderZOrigin();
  ~RTExtraInfoProviderZOrigin(){}; 
  virtual void PreUserTrackingAction( const G4Track* aTrack );

private:
};

#endif
