#include "GmORFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmORFilter::GmORFilter(G4String name)
  :GmVFilterOfFilters(name)
{
}

//----------------------------------------------------------------
GmORFilter::~GmORFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmORFilter::AcceptTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptTrack number of filters " << theFilters.size() << G4endl;
#endif
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptTrack filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptTrack(aTrack) ) return TRUE;
  }
  return FALSE;
}


//----------------------------------------------------------------
G4bool GmORFilter::AcceptStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptStep number of filters " << theFilters.size() << G4endl;
#endif
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptStep filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptStep(aStep) ) return TRUE;
  }
  return FALSE;
}
