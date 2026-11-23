#ifndef GmVPositionOrigin_HH
#define GmVPositionOrigin_HH

#include <vector>
#include "globals.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"

class GmVPositionOrigin
{
public:
  GmVPositionOrigin(){};
  ~GmVPositionOrigin(){};

  virtual std::vector<GVSTouchableInfo*> AddTouchableInfos( const std::vector<G4String>& params ) const = 0;

private:
};

#endif
