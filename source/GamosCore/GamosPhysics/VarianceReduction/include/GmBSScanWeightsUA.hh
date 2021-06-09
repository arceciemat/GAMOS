#ifndef __GmBSScanWeightsUA__
#define __GmBSScanWeightsUA__

class GmBSTrajectory;
class GmAnalysisMgr;
#include "globals.hh"
#include <vector>
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmBSScanWeightsUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmBSScanWeightsUA();
  ~GmBSScanWeightsUA(){}; 

  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack );
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfEventAction( const G4Event* );

  void DefineHistoLimits();
  void BookHistos(G4String type);
  void FillHistos( GmBSTrajectory* traj, G4int hid );

  GmBSTrajectory* GetParent(GmBSTrajectory* traj) const; 

private:
  G4int theNbins;
  G4double theHisXY;
  G4double theHisZ;
  G4double theHisE;
  std::vector<GmBSTrajectory*> theTrajectories;
  GmAnalysisMgr* theAnaMgr;

  G4double thePlaneZ;
  G4double thePlaneHalfWidth;
  G4bool bPrimary;
};

#endif
