
#ifndef GmReadPhantomEGSGeometry_h
#define GmReadPhantomEGSGeometry_h 1

#include "globals.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
class GmFileIn;

class GmReadPhantomEGSGeometry : public GmReadPhantomGeometry
{
public:

  GmReadPhantomEGSGeometry();
  ~GmReadPhantomEGSGeometry();

  virtual void ReadPV( GmFileIn& ){};

private:
  virtual void ReadPhantomData();

  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 

};

#endif
