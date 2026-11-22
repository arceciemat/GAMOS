#ifndef GmProcessFilter_h
#define GmProcessFilter_h 1

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

class GmProcessFilter : public GmVFilter 
{
public:
  GmProcessFilter(G4String name);
  virtual ~GmProcessFilter();

  virtual G4bool AcceptStep(const G4Step* aStep);

  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  std::set<G4VProcess*> theProcesses;
  
};

#endif
