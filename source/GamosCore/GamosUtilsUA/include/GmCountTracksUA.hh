#ifndef __GmCountTracksUA__
#define __GmCountTracksUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"

class G4Run;
class G4Event;
class G4Track;

class GmCountTracksUA : public GmUserRunAction, 
			public GmUserEventAction, 
			public GmUserTrackingAction
{
public:
  GmCountTracksUA();
  ~GmCountTracksUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void PostUserTrackingAction(const G4Track* aTrack);

private:
  G4double theNTracks;
  G4int theNTracksThisEvent;
  G4int theEachNEvent;
  G4int theFirstEvent;

};

#endif
