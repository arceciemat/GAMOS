#ifndef GmHistoryAllFilter_h
#define GmHistoryAllFilter_h 1

#include "globals.hh"
#include "GmVANDFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in all previous ones including this 
// 
///////////////////////////////////////////////////////////////////////////////

class GmHistoryAllFilter : public GmVANDFilter
{

public: // with description
  GmHistoryAllFilter(G4String name);
  
  virtual ~GmHistoryAllFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

private:
  G4bool bNotPassed; // not passed the filters in any previous step, or PreTrack
};

#endif
