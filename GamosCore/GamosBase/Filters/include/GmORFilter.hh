#ifndef GmORFilter_h
#define GmORFilter_h 1

#include "globals.hh"
#include "GmVFilterOfFilters.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters to filter on any of them
// 
///////////////////////////////////////////////////////////////////////////////

class GmORFilter : public GmVFilterOfFilters
{

public: // with description
  GmORFilter(G4String name);
  
  virtual ~GmORFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
 
};

#endif
