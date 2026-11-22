#ifndef __PETHistosPositron__
#define __PETHistosPositron__

#include "GamosCore/GamosSD/include/GmSDMessenger.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class G4Event;
class GmAnalysisMgr;

class PETHistosPositron : public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  PETHistosPositron();
  ~PETHistosPositron(){};
  void PreUserTrackingAction(const G4Track* aTrack);
  void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  GmAnalysisMgr* theAnaMgr;

};

#endif
