#ifndef __RTVExtraInfoProviderLongByRegion__
#define __RTVExtraInfoProviderLongByRegion__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "RTVExtraInfoProviderLong.hh"
class GmRegionCutsMgr;

class RTVExtraInfoProviderLongByRegion : public RTVExtraInfoProviderLong
{
public:
  RTVExtraInfoProviderLongByRegion();
  ~RTVExtraInfoProviderLongByRegion(){}; 

  void SetWordIndexFromRegion( const G4Track* aTrack );

  virtual G4String GetIndexName( G4int index ) const;

  virtual void DumpIndices(std::ostream& out);

protected:
  GmRegionCutsMgr* theRegionCutsMgr;
};

#endif
