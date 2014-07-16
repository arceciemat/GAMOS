#ifndef GmRangeFilter_h
#define GmRangeFilter_h 1

#include "globals.hh"
#include "GmInitialRangeFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if range is in defined limits
//
///////////////////////////////////////////////////////////////////////////////

class GmRangeFilter : public GmInitialRangeFilter
{
  
  //-------
public: // with description
  GmRangeFilter(G4String name);
  // Constructor
  
  virtual ~GmRangeFilter();
  public: // with description
  virtual G4bool AcceptStep(const G4Step*);

};

#endif
