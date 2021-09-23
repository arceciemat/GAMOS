#include "RTExtraInfoProviderZOrigin.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderZOrigin::RTExtraInfoProviderZOrigin()
{
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZOrigin::PreUserTrackingAction( const G4Track* aTrack )
{
  *theWord = aTrack->GetVertexPosition().z();

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoProviderZOrigin ( aTrack ) " << *theWord << G4endl;
#endif

}
