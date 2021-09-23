#include "RTExtraInfoProviderInteractions.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"
#include "G4ProcessType.hh"

//---------------------------------------------------------------------
RTExtraInfoProviderInteractions::RTExtraInfoProviderInteractions()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderInteractions::PreUserTrackingAction( const G4Track*  )
{
  if( bWordsReset ) {
    for( unsigned int ii = 0; ii < theWords.size(); ii++ ) {
      theWords[ii]->reset();
    }
  }
}
 

//---------------------------------------------------------------------
void RTExtraInfoProviderInteractions::UserSteppingAction( const G4Step* aStep )
{
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType != fElectromagnetic 
      && procType != fOptical
      && procType != fHadronic
      && procType != fPhotolepton_hadron
      && procType != fDecay ) {
    return;
  }

  G4Track* aTrack = aStep->GetTrack();
  SetWordIndexFromRegion( aTrack );
}
