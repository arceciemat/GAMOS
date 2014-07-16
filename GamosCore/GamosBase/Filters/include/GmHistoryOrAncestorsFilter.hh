#ifndef GmHistoryOrAncestorsFilter_h
#define GmHistoryOrAncestorsFilter_h 1

#include "GmVANDFilter.hh"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step or any previous one of any of its ancestors (if it passed, it does not check again)
// 
///////////////////////////////////////////////////////////////////////////////

class GmHistoryOrAncestorsFilter : public GmVANDFilter
{

public: // with description
  GmHistoryOrAncestorsFilter(G4String name);
  
  virtual ~GmHistoryOrAncestorsFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  std::set<G4int> bPassed; // list of tracks (ancestors) that passed the filters in any previous step, or PreTrack
};

#endif
