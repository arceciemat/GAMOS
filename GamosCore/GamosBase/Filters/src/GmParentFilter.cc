#include "GmParentFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"

//----------------------------------------------------------------
GmParentFilter::GmParentFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmParentFilter::~GmParentFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmParentFilter::AcceptTrack(const G4Track* aTrack)
{
  G4int parentID = aTrack->GetParentID();
  if( parentID == 0 &&
      aTrack->GetCurrentStepNumber() == 0 ) bPassed.clear();

  G4bool bAccept = AcceptTrackAND(aTrack);
  if( bAccept ) bPassed.insert(aTrack->GetTrackID());

  if( bPassed.find( parentID ) != bPassed.end()) { // passed parent in any step
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptTrack return TRUE, because ancestor was accepted " << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptTrack return FALSE" << G4endl;
#endif

  return FALSE;

}


//----------------------------------------------------------------
G4bool GmParentFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = FALSE;

  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  
  if( aTrack->GetParentID() == 0 &&
      aTrack->GetCurrentStepNumber() == 1 ) bPassed.clear();

  //--- Check if this step passes
  G4int trkID = aTrack->GetTrackID();
  if( !bAccept ) {
    bAccept = AcceptStepAND(aStep);
    if( bAccept ) bPassed.insert(trkID);
  } 

  //--- Check if passed previous track at AcceptTrack
  if( bPassed.find( aTrack->GetParentID()) != bPassed.end()) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptStep returns TRUE, because ancestor was accepted at AcceptTrack " << G4endl;
#endif
    return TRUE;
  } 
    
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmParentFilter::AcceptStep return FALSE" << G4endl;
#endif

  return FALSE;

}
