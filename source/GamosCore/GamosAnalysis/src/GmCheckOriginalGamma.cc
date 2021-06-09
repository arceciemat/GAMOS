#include "GmCheckOriginalGamma.hh"
#include "GmAnalysisVerbosity.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

std::set<G4int> GmCheckOriginalGamma::theOrigGammas;
std::map<G4int,G4int> GmCheckOriginalGamma::theTrackAndParentIDs;
std::map<G4int,G4bool> GmCheckOriginalGamma::isFromRadiactiveDecay;
G4int GmCheckOriginalGamma::bCheckParentID = true;

GmCheckOriginalGamma::GmCheckOriginalGamma()
{
}

G4int GmCheckOriginalGamma::CheckIfOriginalGamma(const G4Track* aTrack)
{
  G4int isOriginalGamma = 0;

  if( aTrack->GetDefinition()->GetParticleName() == "gamma"  ) {
    if( aTrack->GetCreatorProcess() == 0  ) {
      isOriginalGamma = 1;
    } else if( aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" ) { // gamma from radioactive decay of isotope that was primary particle
      if(theTrackAndParentIDs[aTrack->GetParentID()] == 1 
	 || theTrackAndParentIDs[ theTrackAndParentIDs[aTrack->GetParentID()]-1 ] == 1) { // gamma from radioactive decay may be done through an excited isotope
	isOriginalGamma = 1;
      }
    } else if( aTrack->GetCreatorProcess()->GetProcessName() == "annihil" ) {
      if(theTrackAndParentIDs[aTrack->GetParentID()] == 1 ) { 	// gamma from annihilation of e+ that was primary particle
	isOriginalGamma = 2;
      } else if(isFromRadiactiveDecay[aTrack->GetParentID()] == 1 && 
		theTrackAndParentIDs[ theTrackAndParentIDs[aTrack->GetParentID()]-1 ] == 1) { 	// gamma from annihilation of e+ that comes from radioactive decay of isotope that was primary particle
	isOriginalGamma = 2;
      } 
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( AnaVerb(infoVerb) ) G4cout << " GmCheckOriginalGamma::IsOriginalGamma " << isOriginalGamma << G4endl;
#endif

  return isOriginalGamma;

}

void GmCheckOriginalGamma::BeginOfEventAction( const G4Event* )
{
  GmCheckOriginalGamma::theOrigGammas.clear();
  GmCheckOriginalGamma::theTrackAndParentIDs.clear();
  GmCheckOriginalGamma::isFromRadiactiveDecay.clear();
}

void GmCheckOriginalGamma::PreUserTrackingAction(const G4Track* aTrack)
{
  if( CheckIfOriginalGamma( aTrack ) != 0 ) GmCheckOriginalGamma::theOrigGammas.insert(aTrack->GetTrackID());

  GmCheckOriginalGamma::theTrackAndParentIDs[aTrack->GetTrackID()] = aTrack->GetParentID()+1;

  G4bool isFromRD = false;
  if( aTrack->GetCreatorProcess() != 0 && aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" ) isFromRD = true;
  GmCheckOriginalGamma::isFromRadiactiveDecay[aTrack->GetTrackID()] = isFromRD; 
}


G4int GmCheckOriginalGamma::GetOriginalGamma(const G4Track* aTrack)
{
  G4int trkid = aTrack->GetTrackID();
  G4int origGamma = 0;
  do{ 
    if( trkid < 0 ) return 0; // tracks postponed to other event
    if( theOrigGammas.find(trkid) != theOrigGammas.end() ) {
      origGamma = trkid;
      break;
    }
    // do not check this, because it will give an error if a track is postponed to next event
    if( theTrackAndParentIDs.find(trkid) == theTrackAndParentIDs.end() ) {
     if( bCheckParentID ) {
	G4Exception("GmCheckOriginalGamma::GetOriginalGamma",
		    "GAMOS internal error",
		    FatalException,
		    "Track ID not found in theTrackAndParentIDs");
      } else {
	return 0;
      }
    }
    trkid = theTrackAndParentIDs[trkid]-1;
  } while( trkid != 0 );

#ifndef GAMOS_NO_VERBOSE
   if( AnaVerb(infoVerb) ) G4cout << " GmCheckOriginalGamma::GetOriginalGamma " << origGamma << G4endl;
#endif

   return origGamma;
}
