#ifndef GmCrossPhantomGeometry_h
#define GmCrossPhantomGeometry_h 1

#include "globals.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
#include <vector>
#include <map>
class GmFileIn;

class GmCrossPhantomGeometry : public GmReadPhantomGeometry
{
public:

  GmCrossPhantomGeometry();
  ~GmCrossPhantomGeometry();

  virtual void ConstructPhantom(G4LogicalVolume* parentLV);

  virtual void ReadPS( GmFileIn& ){};

private:
  virtual void ReadPhantomData();

private:
  std::vector<G4String> theMaterialNames;
  std::map<G4int,G4int> theMaterialZVoxels;
  std::vector<G4double> theMaterialDensities;

};

#endif
