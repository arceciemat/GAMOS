#ifndef GmPositionG4Materials_HH
#define GmPositionG4Materials_HH

#include <vector>
#include "globals.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionOrigin.hh"

class GmPositionG4Materials : public GmVPositionOrigin
{
public:
  GmPositionG4Materials();
  ~GmPositionG4Materials(){};

  virtual std::vector<GVSTouchableInfo*> AddTouchableInfos( const std::vector<G4String>& params ) const;

private:
};

#endif
