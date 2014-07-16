#ifndef GmOnSecondaryFilter_h
#define GmOnSecondaryFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and applies them to the secondary particles generated, so that at least one must pass them all
// 
///////////////////////////////////////////////////////////////////////////////

class GmOnSecondaryFilter : public GmVANDFilter
{

public: // with description
  GmOnSecondaryFilter(G4String name);
  
  virtual ~GmOnSecondaryFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
 
};

#endif
