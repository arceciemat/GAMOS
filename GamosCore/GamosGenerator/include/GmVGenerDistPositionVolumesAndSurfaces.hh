#ifndef GmVGenerDistPositionVolumesAndSurfaces_HH
#define GmVGenerDistPositionVolumesAndSurfaces_HH
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class G4VSolid;
class G4TouchableHistory;

struct GVSTouchableInfo{
  G4String name;
  const G4VSolid* solid;
  G4ThreeVector pos;
  G4RotationMatrix rotmat;
};

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "GmVPositionObjectPos.hh"
#include "GmVPositionOrigin.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmGeometryUtils;
class GmVGenerDistPositionVolumesAndSurfaces : public GmVGenerDistPosition
{ 
public:
  GmVGenerDistPositionVolumesAndSurfaces();
  virtual ~GmVGenerDistPositionVolumesAndSurfaces();

  virtual void SetParams( const std::vector<G4String>& params );

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

protected:
  std::vector<GVSTouchableInfo*> theTouchableInfos;
  std::vector<G4double> theTotalObjectDimensions;

  GmVPositionObjectPos* theObject;
  GmVPositionOrigin* theOrigin;

  G4bool bOnlyVolume;
  GmGeometryUtils* theGeomUtils;
  G4TouchableHistory* theTouchable;

};


#endif
