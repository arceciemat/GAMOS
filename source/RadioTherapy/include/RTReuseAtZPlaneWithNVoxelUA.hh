#ifndef __RTReuseAtZPlaneWithNVoxelUA__
#define __RTReuseAtZPlaneWithNVoxelUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include <map>
#include <set>

class RTReuseAtZPlaneWithNVoxelUA : public GmUserRunAction, public GmUserEventAction, public GmUserSteppingAction, public GmUserTrackingAction
{
public:
  RTReuseAtZPlaneWithNVoxelUA();
  ~RTReuseAtZPlaneWithNVoxelUA();

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void PreUserTrackingAction(const G4Track* aTrack );
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfRunAction(const G4Run* aRun);
  virtual void EndOfEventAction(const G4Event* anEvent);

private:
  std::map<G4int, std::set<G4int> > theNVoxelList;
  G4double theRatioSum;
  G4double theRatioSum2;
  G4double theZPlane;
  G4int theReuseTrackID;
  std::set<G4int> theNEvents; // events where at least one step is counted
  std::set<G4int> theGoodVoxels; // events where at least one step is counted 
};

#endif
