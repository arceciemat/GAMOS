#ifndef GmVPositionObjectPos_HH
#define GmVPositionObjectPos_HH

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UImessenger.hh"
#include <map>
class G4VSolid;
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"

class GmVPositionObjectPos 
{
public:
  GmVPositionObjectPos(){};
  ~GmVPositionObjectPos(){};

  virtual G4double GetObjectDimension(const G4VSolid* solid) const = 0;
  virtual G4ThreeVector GeneratePosition( const GVSTouchableInfo* tinfo ) = 0;
  virtual void BuildSurfaceAreas( const G4VSolid* ) = 0;

};

#endif
