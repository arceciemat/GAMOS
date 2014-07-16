#ifndef GmInMassLogicalVolumeFilter_h
#define GmInMassLogicalVolumeFilter_h 1

class G4LogicalVolmue;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "GmTouchableFilterLogicalVolume.hh"
////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by logical volume, returning always the mass volumethe particle is in, even if parallel geometry exists
// 
///////////////////////////////////////////////////////////////////////////////

class GmInMassLogicalVolumeFilter : public GmVFilter, GmTouchableFilterLogicalVolume
{

public: // with description
  GmInMassLogicalVolumeFilter(G4String name);
  
  virtual ~GmInMassLogicalVolumeFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);

  virtual void SetParameters( std::vector<G4String>& params);
  virtual void show();
  
};

#endif
