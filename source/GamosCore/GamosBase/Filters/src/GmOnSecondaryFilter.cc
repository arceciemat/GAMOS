#include "GmOnSecondaryFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"

//----------------------------------------------------------------
GmOnSecondaryFilter::GmOnSecondaryFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmOnSecondaryFilter::~GmOnSecondaryFilter()
{ 
}


//----------------------------------------------------------------
G4bool GmOnSecondaryFilter::AcceptStep(const G4Step* )
{
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep  number of filters " << theFilters.size() << G4endl;
#endif
  G4TrackVector::const_iterator ite;
  for( ite = secos.begin(); ite != secos.end(); ite++ ) {
    // Check if one of the secondary pass filters 
    if( AcceptTrackAND(*ite) ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep return TRUE " << G4endl;
#endif
      return TRUE; // one secondary passed all the filters
    }
  }
    
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep  return FALSE " << G4endl;
#endif
  
  return FALSE;

}

//----------------------------------------------------------------
G4bool GmOnSecondaryFilter::AcceptTrack(const G4Track* secoTrack)
{
  // assumes the track is a secondary track (user should check it)
   
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack  number of filters " << theFilters.size() << G4endl;
#endif
  if( AcceptTrackAND(secoTrack) ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack return TRUE " << G4endl;
#endif
    return TRUE; // one secondary passed all the filters
  }
    
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack  return FALSE " << G4endl;
#endif
  
  return FALSE;
  
}

//-------------------------------------------------------------------------
G4bool GmOnSecondaryFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmOnSecondaryFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
