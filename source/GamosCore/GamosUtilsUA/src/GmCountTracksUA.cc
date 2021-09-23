#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "GmCountTracksUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmCountTracksUA::GmCountTracksUA()
{
  theNTracks = 0;
  theNTracksThisEvent = 0;
  
}

//----------------------------------------------------------------
void GmCountTracksUA::BeginOfRunAction( const G4Run* )
{

  theEachNEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EachNEvent", 1000));
  theFirstEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":FirstEvent", 0));
}

//----------------------------------------------------------------
void GmCountTracksUA::PostUserTrackingAction(const G4Track* )
{
  theNTracks++;
  theNTracksThisEvent++;
}

//----------------------------------------------------------------
void GmCountTracksUA::EndOfEventAction( const G4Event* event )
{
  G4int evtid = event->GetEventID();
  if( evtid >= theFirstEvent && evtid%theEachNEvent == 0 ) {
    G4cout << " %%% EVENT " << evtid << " NTRACKS " << theNTracksThisEvent << " TOTAL NTRACKS " << theNTracks << G4endl;
  }
  theNTracksThisEvent = 0;
}
