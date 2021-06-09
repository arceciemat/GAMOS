#ifndef __GmVisStoreTrajUA__
#define __GmVisStoreTrajUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include <set>

class GmVisStoreTrajUA : public GmUserEventAction , public GmUserTrackingAction
{
public:
  GmVisStoreTrajUA();
  ~GmVisStoreTrajUA(){}; 
  virtual void BeginOfEventAction( const G4Event* aEvent );

private:
  G4int theNEvents;
  std::set<G4int> theEventList;

};

#endif
