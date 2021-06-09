#ifndef GmInteractionFilter_h
#define GmInteractionFilter_h 1

class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by process name
// 
///////////////////////////////////////////////////////////////////////////////

class GmInteractionFilter : public GmVFilter 
{
public:
  GmInteractionFilter(G4String name);
  virtual ~GmInteractionFilter();

  virtual G4bool AcceptStep(const G4Step* aStep);

};

#endif
