
#ifndef GmSimplePhantomGeometry_h
#define GmSimplePhantomGeometry_h 1

#include "globals.hh"
#include "GmReadPhantomGeometry.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
#include <vector>
#include <map>

class GmSimplePhantomGeometry : public GmReadPhantomGeometry
{
public:

  GmSimplePhantomGeometry();
  ~GmSimplePhantomGeometry();


  virtual void ReadPV( std::ifstream& ){};

private:
  virtual void ReadPhantomData();

private:
  std::vector<G4String> theMaterialNames;
  std::map<G4int,G4int> theMaterialZVoxels;
  std::vector<G4double> theMaterialDensities;

};

#endif
