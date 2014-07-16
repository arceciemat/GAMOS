#ifndef GmXORFilter_h
#define GmXORFilter_h 1

#include "globals.hh"
#include "GmVFilterOfFilters.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters to filter on any of them if only one accets it
// 
///////////////////////////////////////////////////////////////////////////////

class GmXORFilter : public GmVFilterOfFilters
{

public: // with description
  GmXORFilter(G4String name);
  
  virtual ~GmXORFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
 
};

#endif
