#include "GmEventTimeExtractorFromFirstTrack.hh"
#include "G4Event.hh"

G4double GmEventTimeExtractorFromFirstTrack::GetEventTime( const G4Event* evt )
{
  //set it as time of first vertex
  //  G4cout << "  GmEventTimeExtractorFromFirstTrack::GetEventTime " << evt->GetPrimaryVertex(0)->GetT0() << G4endl;
  return evt->GetPrimaryVertex(0)->GetT0();
}
