#ifndef GmPositionSurfacePos_HH
#define GmPositionSurfacePos_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionObjectPos.hh"
#include <map>
class G4VSolid;

class GmPositionSurfacePos : public GmVPositionObjectPos
{
public:
  GmPositionSurfacePos();
  virtual ~GmPositionSurfacePos(){};

  virtual G4double GetObjectDimension(const G4VSolid* solid) const;
  virtual G4ThreeVector GeneratePosition( const GVSTouchableInfo* tinfo ); 
  virtual void BuildSurfaceAreas( const G4VSolid* solid );

private:
  G4ThreeVector GeneratePosInSolidSurface( const G4VSolid* solid );
  unsigned int GetOneArea( const G4VSolid* solid );

private:
  std::map<const G4VSolid*,std::vector<G4double> >  theSurfaceAreas;
};

#endif
