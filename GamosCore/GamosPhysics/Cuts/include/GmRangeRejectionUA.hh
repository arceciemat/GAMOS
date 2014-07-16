#ifndef __GmRangeRejectionUA__
#define __GmRangeRejectionUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include <map>

class G4Run;
class G4Track;
class G4LogicalVolume;

class GmRangeRejectionUA : public GmUserRunAction, public GmUserStackingAction
{
public:
  GmRangeRejectionUA();
  ~GmRangeRejectionUA(){};
  virtual void BeginOfRunAction(const G4Run* );

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);

private:
  G4bool bUseDirection;

};

#endif
