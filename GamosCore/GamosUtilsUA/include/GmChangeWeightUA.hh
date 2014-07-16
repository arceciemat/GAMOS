#ifndef GmChangeWeightUA_h
#define GmChangeWeightUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
class GmVDistribution;

class GmChangeWeightUA : public GmUserRunAction, 
			 public GmUserTrackingAction, 
			 public GmUserSteppingAction
{

 public:
  GmChangeWeightUA();
  ~GmChangeWeightUA();

 public:
  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

 private:
  GmVDistribution* theDistribution;
};

#endif
