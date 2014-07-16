#ifndef GmParentFilter_h
#define GmParentFilter_h 1

#include "GmVANDFilter.hh"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step or any previous one of any of its ancestors (if it passed, it does not check again)
// 
///////////////////////////////////////////////////////////////////////////////

class GmParentFilter : public GmVANDFilter
{

public: // with description
  GmParentFilter(G4String name);
  
  virtual ~GmParentFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  std::set<G4int> bPassed; // list of tracks (ancestors) that passed the filters in any previous step, or PreTrack
};

#endif
