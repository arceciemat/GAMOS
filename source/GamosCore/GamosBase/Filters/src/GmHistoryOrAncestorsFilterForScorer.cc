#include "GmHistoryOrAncestorsFilterForScorer.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmHistoryOrAncestorsFilterForScorer::GmHistoryOrAncestorsFilterForScorer(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryOrAncestorsFilterForScorer::~GmHistoryOrAncestorsFilterForScorer()
{ 
}

//----------------------------------------------------------------
void GmHistoryOrAncestorsFilterForScorer::BeginOfEventAction(const G4Event* )
{
  bPassed.clear();

}

//----------------------------------------------------------------
void GmHistoryOrAncestorsFilterForScorer::PreUserTrackingAction(const G4Track* aTrack)
{
  AcceptTrack(aTrack);
  return;
  
}

//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilterForScorer::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end() ) { // passed a previous step
    //      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << theFilterName <<" GmHistoryOrAncestorsFilterForScorer::AcceptTrack return 1, because ancestor track was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << theFilterName <<" GmHistoryOrAncestorsFilterForScorer::AcceptTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}

//----------------------------------------------------------------
void GmHistoryOrAncestorsFilterForScorer::UserSteppingAction( const G4Step* aStep )
{
  AcceptStep(aStep);
  return;
  
}


//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilterForScorer::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) bPassed.clear();

  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* gmTrkInfo = dynamic_cast<GmTrackInfo*>(trkInfo);

  //  G4cout << " GmHistoryOrAncestorsFilterForScorer::AcceptStep gmTrkInfo " << gmTrkInfo  << G4endl; //GDEB
  //-  if( !gmTrkInfo ) return FALSE;
  //--- Check if passed a previous step of current track or previous track at AcceptTrack
  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end() ) {
    //-      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << theFilterName <<" GmHistoryOrAncestorsFilterForScorer::AcceptStep return 1, because previous step of current track step was accepted " << G4endl;
#endif
    bPassed.insert(trkID);
    bAccept = TRUE;
  } else {

    //--- Check if passed an step of a previous track, that is the step where the current track originated
    if( gmTrkInfo ) {
      if( gmTrkInfo->IntValueExists("HistoryOrAncestors"+theFilterName) ) {
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << theFilterName << " GmHistoryOrAncestorsFilterForScorer::AcceptStep return 1, because ancestor step was accepted " << G4endl;
#endif
	bAccept = TRUE;
      }
    }
  }
 
  //  G4cout << " GmHistoryOrAncestorsFilterForScorer::AcceptStep bAccept1 " << bAccept  << G4endl; //GDEB
  //--- Check if this step passes
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( bAccept ) if( FilterVerb(debugVerb) ) G4cout << theFilterName << " GmHistoryOrAncestorsFilterForScorer::AcceptStep return 1, because current step is accepted " << G4endl;
#endif
  }
  //  G4cout << " GmHistoryOrAncestorsFilterForScorer::AcceptStep bAccept2 " << bAccept  << G4endl; //GDEB

  if( bAccept ) {
    bPassed.insert(aTrack->GetTrackID());
    //--- Set GmTrackInfo of daughter tracks (for current track bPassed is used)
    G4int ancestorLevel = 1;
    if( gmTrkInfo ) {
      G4int il = gmTrkInfo->GetIntValue("HistoryOrAncestors"+theFilterName)+1;
      if( il != INT_MAX ) ancestorLevel = il;
    }
    
    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    for( unsigned int ii = 0; ii < secos.size(); ii++) {
      GmTrackInfo* gmTrkInfo2 = 0;
      if( secos[ii]->GetUserInformation() == 0 ) {
	gmTrkInfo2 = new GmTrackInfo();
      } else {
	G4VUserTrackInformation* g4TrkInfo = secos[ii]->GetUserInformation();
	if( ! dynamic_cast<GmTrackInfo*>(g4TrkInfo) ) {
	  G4cerr << " G4VUserTrackInformation of type " << g4TrkInfo->GetType() << G4endl;
	  G4Exception("GmHistoryOrAncestorsFilterForScorer::AcceptStep",
		      "",
		      FatalException,
		      "Creating a GmTrackInfo for a track that has already a G4VUserTrackInformation");
	} else {
	  gmTrkInfo2 = dynamic_cast<GmTrackInfo*>(g4TrkInfo);
	}
      }
      gmTrkInfo2->SetIntValue("HistoryOrAncestors"+theFilterName,ancestorLevel,false);
      secos[ii]->SetUserInformation(gmTrkInfo2);
      //      G4cout << " SetUserInformation " <<  secos[ii] << " : "<< gmTrkInfo2 << G4endl; //GDEB
    } 
  }
    
  //  G4cout << " GmHistoryOrAncestorsFilterForScorer::AcceptStep bAccept3 " << bAccept  << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << theFilterName << " GmHistoryOrAncestorsFilterForScorer::AcceptStep return " << bAccept << G4endl;
#endif

  return bAccept;

}



//----------------------------------------------------------------
G4bool GmHistoryOrAncestorsFilterForScorer::AcceptStackedTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4int trkID = aTrack->GetTrackID();
  if( bPassed.find(trkID) != bPassed.end()  // passed a previous step
      || bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << theFilterName <<" GmHistoryOrAncestorsFilterForScorer::AcceptStackedTrack return 1, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptStackedTrackAND(aTrack);

  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << theFilterName <<" GmHistoryOrAncestorsFilterForScorer::AcceptStackedTrack return " << bAccept << G4endl;
#endif

  return bAccept;

}
