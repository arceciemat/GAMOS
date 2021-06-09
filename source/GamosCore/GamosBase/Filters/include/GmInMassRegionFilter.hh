#ifndef GmInMassRegionFilter_h
#define GmInMassRegionFilter_h 1

class G4LogicalVolmue;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "GmTouchableFilterRegion.hh"
////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by logical volume, returning always the mass volumethe particle is in, even if parallel geometry exists
// 
///////////////////////////////////////////////////////////////////////////////

class GmInMassRegionFilter : public GmVFilter, GmTouchableFilterRegion
{

public: // with description
  GmInMassRegionFilter(G4String name);
  
  virtual ~GmInMassRegionFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  virtual void SetParameters( std::vector<G4String>& params);
  virtual void show();
  
};

#endif
