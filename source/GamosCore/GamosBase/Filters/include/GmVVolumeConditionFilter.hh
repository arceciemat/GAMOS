#ifndef GmVVolumeConditionFilter_h
#define GmVVolumeConditionFilter_h 1

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
class GmVVolumeConditionFilter : public GmVFilter 
{

public: // with description
  GmVVolumeConditionFilter(G4String name);
  
  virtual ~GmVVolumeConditionFilter();
  
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

#include "GmVVolumeConditionFilter.icc"
#endif
