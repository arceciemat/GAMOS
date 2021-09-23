#include "GmHistoryFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//----------------------------------------------------------------
GmHistoryFilter::GmHistoryFilter(G4String name)
  :GmVANDFilter(name)
{ 
  bPassed = FALSE;
}

//----------------------------------------------------------------
GmHistoryFilter::~GmHistoryFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 )  bPassed = FALSE;

  if( bPassed )  {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptTrack 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( bAccept ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptTrack " << bAccept << G4endl;
#endif

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryFilter::AcceptStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 )  bPassed = FALSE;

  if( bPassed ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptStep 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptStepAND(aStep);

  if( bAccept ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptStep " << bAccept << G4endl;
#endif

  return bAccept;

}

//----------------------------------------------------------------
G4bool GmHistoryFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 )  bPassed = FALSE;

  if( bPassed )  {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptTrack 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAccept = AcceptStackedTrackAND(aTrack);

  if( bAccept ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmHistoryFilter::AcceptTrack " << bAccept << G4endl;
#endif

  return bAccept;

}
