#include "GmXORFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmXORFilter::GmXORFilter(G4String name)
  :GmVFilterOfFilters(name)
{
}

//----------------------------------------------------------------
GmXORFilter::~GmXORFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmXORFilter::AcceptTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptTrack number of filters " << theFilters.size() << G4endl;
#endif
  G4bool bAccept = FALSE;
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptTrack filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptTrack(aTrack) ) {
      if( !bAccept ) {
	bAccept = TRUE;
      } else {
	bAccept = FALSE;
	break;
      }
    }
  }

  return bAccept;
}


//----------------------------------------------------------------
G4bool GmXORFilter::AcceptStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptStep number of filters " << theFilters.size() << G4endl;
#endif
  G4bool bAccept = FALSE;  
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptStep filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptStep(aStep) ) {
      if( !bAccept ) {
	bAccept = TRUE;
      } else {
	bAccept = FALSE;
	break;
      }
    }
  }

  return bAccept;

}
