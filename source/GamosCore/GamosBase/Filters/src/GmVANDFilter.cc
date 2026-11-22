#include "GmVANDFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//----------------------------------------------------------------
GmVANDFilter::GmVANDFilter(G4String name)
  :GmVFilterOfFilters(name)
{
}

//----------------------------------------------------------------
GmVANDFilter::~GmVANDFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmVANDFilter::AcceptTrackAND(const G4Track* aTrack)
{
  G4bool bAccept = FALSE;
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
    if( theFilters[ii]->AcceptTrack(aTrack) ) {
      bAccept = TRUE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptTrackAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
    } else {
      bAccept = FALSE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptTrackAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
      break;
    }
  }

  return bAccept;
}


//----------------------------------------------------------------
G4bool GmVANDFilter::AcceptStepAND(const G4Step* aStep)
{
  G4bool bAccept = FALSE;
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
    if( theFilters[ii]->AcceptStep(aStep) ) {
      bAccept = TRUE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptStepAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
    } else {
      bAccept = FALSE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptStepAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
      break;
    }
  }

  return bAccept;

}

//----------------------------------------------------------------
G4bool GmVANDFilter::AcceptStackedTrackAND(const G4Track* aTrack)
{
  G4bool bAccept = FALSE;
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
    if( theFilters[ii]->AcceptStackedTrack(aTrack) ) {
      bAccept = TRUE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptStackedTrackAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
    } else {
      bAccept = FALSE;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmVANDFilter::AcceptStackedTrackAND filter= " << theFilters[ii]->GetName() << " " << bAccept << G4endl;
#endif
      break;
    }
  }

  return bAccept;

}

