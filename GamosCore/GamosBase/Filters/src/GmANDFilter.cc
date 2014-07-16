#include "GmANDFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmANDFilter::GmANDFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmANDFilter::~GmANDFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmANDFilter::AcceptTrack(const G4Track* aTrack)
{
  G4bool bAccept =  AcceptTrackAND( aTrack );
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << "GmANDFilter::AcceptStep " << bAccept << G4endl;
#endif
  return bAccept;
}


//----------------------------------------------------------------
G4bool GmANDFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = AcceptStepAND( aStep );
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << "GmANDFilter::AcceptStep " << bAccept << G4endl;
#endif
  return bAccept;
}
