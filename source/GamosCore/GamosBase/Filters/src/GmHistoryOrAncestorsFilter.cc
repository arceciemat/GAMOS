#include "GmHistoryOrAncestorsFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmHistoryOrAncestorsFilter::GmHistoryOrAncestorsFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryOrAncestorsFilter::~GmHistoryOrAncestorsFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end()  // passed a previous step
      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptTrack return 1, because ancestor track was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStep bPassed clear " << G4endl;
#endif
    bPassed.clear();
  }

  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* gmTrkInfo = dynamic_cast<GmTrackInfo*>(trkInfo);
  //-  if( !gmTrkInfo ) return FALSE;

  //--- Check if passed a previous step of current track or previous track at AcceptTrack
  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end() ) {
      //      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << this << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStep return 1, because previous step of current track was accepted " << G4endl;
#endif
    bPassed.insert(trkID);
    bAccept = TRUE;
  } else {

    //--- Check if passed an step of a previous track, that is the step where the current track originated
    if( gmTrkInfo ) {
      if( gmTrkInfo->IntValueExists("HistoryOrAncestors"+GetName()) ) {
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return 1, because ancestor step was accepted " << G4endl;
#endif
	bAccept = TRUE;
      }
    }
  }

  //--- Check if this step passes
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( bAccept ) if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return 1, because current step is accepted " << G4endl;
#endif
  }

  if( bAccept ) {
    bPassed.insert(aTrack->GetTrackID());
    //--- Set GmTrackInfo of daughter tracks (for current track bPassed is used)
    G4int ancestorLevel = 1;
    if( gmTrkInfo ) {
      G4int il = gmTrkInfo->GetIntValue("HistoryOrAncestors"+GetName())+1;
      if( il != INT_MAX ) ancestorLevel = il;
    }
    
    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    for( unsigned int ii = 0; ii < secos.size(); ii++) {
      GmTrackInfo* gmTrkInfo2 = 0;
      if( secos[ii]->GetUserInformation() == 0 ) {
	gmTrkInfo2 = new GmTrackInfo();
	//	G4cout << " G4TrackInfo CREATE " << gmTrkInfo2  << G4endl; //GDEB
     } else {
	G4VUserTrackInformation* g4TrkInfo = secos[ii]->GetUserInformation();
	if( ! dynamic_cast<GmTrackInfo*>(g4TrkInfo) ) {
	  G4Exception("GmAncestorsFilter::AcceptStep",
		      "",
		      FatalException,
		      "Creating a GmTrackInfo for a track that has already a G4VUserTrackInformation");
	} else {
	  gmTrkInfo2 = dynamic_cast<GmTrackInfo*>(g4TrkInfo);
	}
      }
      gmTrkInfo2->SetIntValue("HistoryOrAncestors"+GetName(),ancestorLevel,true);
      secos[ii]->SetUserInformation(gmTrkInfo2);
    } 
  }
    
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryOrAncestorsFilter::AcceptStep return " << bAccept << G4endl;
#endif

  return bAccept;

}

//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end()  // passed a previous step
      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStackedTrack return 1, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptStackedTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() <<" GmHistoryOrAncestorsFilter::AcceptStackedTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}


