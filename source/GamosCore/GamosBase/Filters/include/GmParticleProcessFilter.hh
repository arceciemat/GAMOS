#ifndef GmParticleProcessFilter_h
#define GmParticleProcessFilter_h 1

class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by particle+process name
// 
///////////////////////////////////////////////////////////////////////////////

class GmParticleProcessFilter : public GmVFilter 
{
public:
  GmParticleProcessFilter(G4String name);
  virtual ~GmParticleProcessFilter();

  virtual G4bool AcceptStep(const G4Step* aStep);
  
  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  std::set<G4VProcess*> theProcesses;

};

#endif
