#ifndef GmPositionG4Volumes_HH
#define GmPositionG4Volumes_HH

#include <vector>
#include "globals.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionOrigin.hh"

class GmPositionG4Volumes : public GmVPositionOrigin
{
public:
  GmPositionG4Volumes();
  ~GmPositionG4Volumes(){};

  virtual std::vector<GVSTouchableInfo*> AddTouchableInfos( const std::vector<G4String>& params ) const;

private:
};

#endif
