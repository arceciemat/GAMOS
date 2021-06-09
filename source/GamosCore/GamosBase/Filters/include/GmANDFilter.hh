#ifndef GmANDFilter_h
#define GmANDFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters to filter on all of them
// 
///////////////////////////////////////////////////////////////////////////////

class GmANDFilter : public GmVANDFilter
{

public: // with description
  GmANDFilter(G4String name);
  
  virtual ~GmANDFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
 
};

#endif
