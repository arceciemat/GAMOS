#ifndef GmPastHistoryFilter_h
#define GmPastHistoryFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step or any previous one (if it passed, it does not check again)
// 
///////////////////////////////////////////////////////////////////////////////

class GmPastHistoryFilter : public GmVANDFilter
{

public: // with description
  GmPastHistoryFilter(G4String name);
  
  virtual ~GmPastHistoryFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  G4bool bPassed; // passed the filters in any previous step, or PreTrack
};

#endif
