#ifndef GmVANDFilter_h
#define GmVANDFilter_h 1

#include "globals.hh"
#include "GmVFilterOfFilters.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters to filter on all of them
// 
///////////////////////////////////////////////////////////////////////////////

class GmVANDFilter : public GmVFilterOfFilters
{

public: // with description
  GmVANDFilter(G4String name);
  
  virtual ~GmVANDFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* ) = 0; 
  virtual G4bool AcceptStep(const G4Step* ) = 0;

  virtual G4bool AcceptTrackAND(const G4Track* ); 
  virtual G4bool AcceptStepAND(const G4Step* );
 
};

#endif
