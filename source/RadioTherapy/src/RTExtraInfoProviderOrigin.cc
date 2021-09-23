#include "RTExtraInfoProviderOrigin.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderOrigin::RTExtraInfoProviderOrigin()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderOrigin::PreUserTrackingAction( const G4Track* aTrack )
{
  if( bWordsReset ) {
    for( unsigned int ii = 0; ii < theWords.size(); ii++ ) {
      theWords[ii]->reset();
    }
  }
 
  SetWordIndexFromRegion( aTrack );

}
