#include "GmPrimaryOnAllSecondariesFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------
GmPrimaryOnAllSecondariesFilter::GmPrimaryOnAllSecondariesFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmPrimaryOnAllSecondariesFilter::~GmPrimaryOnAllSecondariesFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptTrack(const G4Track*)
{

  G4SteppingManager* theSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  const G4TrackVector* secos = theSteppingManager->GetSecondary();

  return AcceptAllSecoTracks( *secos, "AcceptTrack" );

}


//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptStep(const G4Step* )
{
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::AcceptStep number of secondaries " << secos.size() << " number of filters " << theFilters.size() << G4endl;
#endif
  return AcceptAllSecoTracks( secos, "AcceptStep" );

}

//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptAllSecoTracks( const G4TrackVector& secos, const G4String& name )
{

  G4bool bAcceptAll = FALSE;
  for( unsigned int ii = 0; ii < secos.size(); ii++) {
    G4bool bAccept = AcceptTrackAND(secos[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::" << name << " seco accepted " << ii << " return 1 " << G4endl;
#endif
    if( bAccept ) {
      bAcceptAll = TRUE; // one secondary passed all the filters
    } else {
      bAcceptAll = FALSE; // one secondary did not pass all the filters
      break;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::" << name << " return " << bAcceptAll << G4endl;
#endif

  return bAcceptAll;

}


//-------------------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmPrimaryOnAllSecondariesFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
