#ifndef GmHistoryAncestorsAllFilter_h
#define GmHistoryAncestorsAllFilter_h 1

#include "GmVANDFilter.hh"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step and all previous one of any of its ancestors (if in one did not pass, it does not check again)
// 
///////////////////////////////////////////////////////////////////////////////

class GmHistoryAncestorsAllFilter : public GmVANDFilter
{

public: // with description
  GmHistoryAncestorsAllFilter(G4String name);
  
  virtual ~GmHistoryAncestorsAllFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  std::set<G4int> bNotPassed; // list of tracks (ancestors) that did not pass the filters in a previous step, or PreTrack
};

#endif
