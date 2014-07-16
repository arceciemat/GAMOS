#include "GmHistoryAllFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmHistoryAllFilter::GmHistoryAllFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmHistoryAllFilter::~GmHistoryAllFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmHistoryAllFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetCurrentStepNumber() == 0 ) bNotPassed = TRUE;

  if( !bNotPassed ) return FALSE;

  G4bool bAccept = AcceptTrackAND(aTrack);

  if( !bAccept ) bNotPassed = FALSE;

  return bAccept;

}


//----------------------------------------------------------------
G4bool GmHistoryAllFilter::AcceptStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1 ) bNotPassed = TRUE;

  if( !bNotPassed ) return FALSE; // if one step failed, do not continue checking

  G4bool bAccept = AcceptStepAND(aStep);

  if( !bAccept ) bNotPassed = FALSE;

  return bAccept;

}
