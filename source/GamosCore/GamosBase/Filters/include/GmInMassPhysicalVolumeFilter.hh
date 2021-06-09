#ifndef GmInMassPhysicalVolumeFilter_h
#define GmInMassPhysicalVolumeFilter_h 1

class G4VPhysicalVolmue;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "GmTouchableFilterPhysicalVolume.hh"
////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by physical volume, returning always the mass volumethe particle is in, even if parallel geometry exists
// 
///////////////////////////////////////////////////////////////////////////////

class GmInMassPhysicalVolumeFilter : public GmVFilter, GmTouchableFilterPhysicalVolume
{

public: // with description
  GmInMassPhysicalVolumeFilter(G4String name);
  
  virtual ~GmInMassPhysicalVolumeFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  virtual void SetParameters( std::vector<G4String>& params);
  virtual void show();
  
};

#endif
