#include "RTExtraInfoProviderCrossings.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderCrossings::RTExtraInfoProviderCrossings()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderCrossings::PreUserTrackingAction( const G4Track* )
{
  if( bWordsReset ) {
    for( unsigned int ii = 0; ii < theWords.size(); ii++ ) {
      theWords[ii]->reset();
    }
  }
}
 

//---------------------------------------------------------------------
void RTExtraInfoProviderCrossings::UserSteppingAction( const G4Step* aStep )
{
  G4Track* aTrack = aStep->GetTrack();
  SetWordIndexFromRegion( aTrack );
}
