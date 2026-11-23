#include "GmEventTimeExtractorFromHit.hh"

G4double GmEventTimeExtractorFromHit::GetEventTime( const G4Event* )
{
  return theEventTime;
}
