#include "GmPastHistoryFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmPastHistoryFilter::GmPastHistoryFilter(G4String name)
  :GmVANDFilter(name)
{ 
  bPassed = FALSE;
}

//----------------------------------------------------------------
GmPastHistoryFilter::~GmPastHistoryFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmPastHistoryFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 )  bPassed = FALSE;

  if( bPassed )  {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << theName << " GmPastHistoryFilter::AcceptTrack 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout <<  theName <<" GmPastHistoryFilter::AcceptTrack " << bPassed << G4endl;
#endif

  return bPassed;

}


//----------------------------------------------------------------
G4bool GmPastHistoryFilter::AcceptStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 )  bPassed = FALSE;

  if( bPassed ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << theName << " GmPastHistoryFilter::AcceptStep 1 , because previously 1" << G4endl;
#endif
    return TRUE;
  }

  G4bool bAcceptOld = AcceptStepAND(aStep);
  if( bAcceptOld ) bPassed = TRUE;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << " GmPastHistoryFilter::AcceptStep 0 " << G4endl;
#endif

  return FALSE;

}
