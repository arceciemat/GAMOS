#ifndef GmTrackRandomWeightUA_hh
#define GmTrackRandomWeightUA_hh 1

#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmTrackRandomWeightUA : public GmUserTrackingAction, 
		      public GmUserRunAction
{
public:   

  GmTrackRandomWeightUA();
  virtual ~GmTrackRandomWeightUA();
  
  virtual void BeginOfRunAction( const G4Run* aRun );
  virtual void PreUserTrackingAction( const G4Track* aTrack );

private:
  G4double theMinWeight;
  G4double theMaxWeight;
  G4double theWeightSpan;


};


#endif
