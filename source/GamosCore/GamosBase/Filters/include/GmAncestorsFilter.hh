#ifndef GmAncestorsFilter_h
#define GmAncestorsFilter_h 1

#include "GmVANDFilter.hh"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step or any previous one of any of its ancestors (if it passed, it does not check again)
// 
///////////////////////////////////////////////////////////////////////////////

class GmAncestorsFilter : public GmVANDFilter
{

public: // with description
  GmAncestorsFilter(G4String name);
  
  virtual ~GmAncestorsFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
  virtual G4bool AcceptStackedTrack(const G4Track* aTrack);

private:
  std::set<G4int> bPassed; // list of tracks (ancestors) that passed the filters in any previous step, or PreTrack
};

#endif
