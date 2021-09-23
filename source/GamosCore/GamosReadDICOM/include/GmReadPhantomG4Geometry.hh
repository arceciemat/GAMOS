
#ifndef GmReadPhantomG4Geometry_h
#define GmReadPhantomG4Geometry_h 1

#include "globals.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;


class GmReadPhantomG4Geometry : public GmReadPhantomGeometry
{
public:

  GmReadPhantomG4Geometry();
  ~GmReadPhantomG4Geometry();

  virtual void ReadPV( GmFileIn& ){};
  G4String GetPatientPosition() const {
    return thePatientPosition;
  }

public:
  virtual void ReadPhantomData();

private:
  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 

  G4String thePatientPosition;
};

#endif
