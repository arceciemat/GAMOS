#ifndef GmPrimaryOn1SecondaryFilter_h
#define GmPrimaryOn1SecondaryFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and applies them to the secondary particles generated, so that at all secondaries must pass all the filters
// 
///////////////////////////////////////////////////////////////////////////////

class GmPrimaryOn1SecondaryFilter : public GmVANDFilter
{

public: // with description
  GmPrimaryOn1SecondaryFilter(G4String name);
  
  virtual ~GmPrimaryOn1SecondaryFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  G4bool Accept1SecoTrack( const G4TrackVector& secos, const G4String& name );
 
};

#endif
