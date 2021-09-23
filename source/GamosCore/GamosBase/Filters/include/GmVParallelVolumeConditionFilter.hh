#ifndef GmVParallelVolumeConditionFilter_h
#define GmVParallelVolumeConditionFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by particle definition.
// 
///////////////////////////////////////////////////////////////////////////////

template <class TCond,class TVol>
class GmVParallelVolumeConditionFilter : public GmVFilter 
{

public: // with description
  GmVParallelVolumeConditionFilter(G4String name);
  
  virtual ~GmVParallelVolumeConditionFilter();
  
public: // with description
  virtual G4bool AcceptStep(const G4Step*);
  virtual G4bool AcceptTrack(const G4Track*);
  
  //
  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  TCond* theCondition;
  TVol* theFilterVolume;

};

#include "GmVParallelVolumeConditionFilter.icc"
#endif
