
#ifndef GmReadPhantomG4BinGeometry_h
#define GmReadPhantomG4BinGeometry_h 1

#include "globals.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include <fstream>

class GmReadPhantomG4BinGeometry : public GmReadPhantomGeometry
{
public:

  GmReadPhantomG4BinGeometry();
  ~GmReadPhantomG4BinGeometry();

  virtual void ReadPV( GmFileIn& ){};

private:
  virtual void ReadPhantomData();

  void ReadVoxelDensitiesBin( FILE* fin );

  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin );

  G4String thePatientPosition;
};

#endif
