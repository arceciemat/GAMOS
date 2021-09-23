#include "RTExtraInfoProviderZLast.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderZLast::RTExtraInfoProviderZLast()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLast::UserSteppingAction( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetCurrentStepNumber() == 1 ) {
    *theWord = aTrack->GetVertexPosition().z();
    theWordPrev = *theWord;
  }

  theWordPrev = *theWord;
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType != fNotDefined
      && procType !=  fTransportation ) { 
    *theWord = aTrack->GetPosition().z();
  }

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoProviderZLast ( aTrack ) " << *theWord << G4endl;
#endif
  
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLast::CheckZStop( G4double zstop )
{
  if( *theWord > zstop ) *theWord = theWordPrev;
}
