#ifndef GmCreatorProcessFilter_h
#define GmCreatorProcessFilter_h 1

class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by creator process name
// 
///////////////////////////////////////////////////////////////////////////////

class GmCreatorProcessFilter : public GmVFilter 
{
public: 
  GmCreatorProcessFilter(G4String name);
  virtual ~GmCreatorProcessFilter();
  
  virtual G4bool AcceptTrack(const G4Track* aTrack);
  
  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  std::set<G4VProcess*> theProcesses;

};

#endif
