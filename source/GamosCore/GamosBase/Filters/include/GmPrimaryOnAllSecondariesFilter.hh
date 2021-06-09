#ifndef GmPrimaryOnAllSecondariesFilter_h
#define GmPrimaryOnAllSecondariesFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and applies them to the secondary particles generated, so that at all secondaries must pass all the filters
// 
///////////////////////////////////////////////////////////////////////////////

class GmPrimaryOnAllSecondariesFilter : public GmVANDFilter
{

public: // with description
  GmPrimaryOnAllSecondariesFilter(G4String name);
  
  virtual ~GmPrimaryOnAllSecondariesFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  G4bool AcceptAllSecoTracks( const G4TrackVector& secos, const G4String& name ); 
};

#endif
