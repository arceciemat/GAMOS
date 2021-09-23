#ifndef GmReadPhantomPartialG4Geometry_h
#define GmReadPhantomPartialG4Geometry_h 1

#include "globals.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <map>
#include <set>

class G4PartialPhantomParameterisation;
class G4LogicalVolume;


class GmReadPhantomPartialG4Geometry : public GmReadPhantomGeometry
{
public:

  GmReadPhantomPartialG4Geometry();
  ~GmReadPhantomPartialG4Geometry();

  virtual void ReadPV( GmFileIn& ){};

private:
  virtual void ReadPhantomData();
  virtual void ConstructPhantom(G4LogicalVolume* parentLV);

  void ReadVoxelDensitiesPartial( std::ifstream& fin, std::map< G4int, std::map< G4int, G4int > > ifxmin, std::map< G4int, std::map< G4int, G4int > > ifxmax );

  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
  void ReadVoxelDensitiesPartial( std::ifstream& fin );

  G4PartialPhantomParameterisation* thePartialPhantomParam;

  std::multimap<G4int,G4int> theFilledIDs;
  std::map< G4int, std::map< G4int, G4int > > theFilledMins;
  std::map< G4int, std::map< G4int, G4int > > theFilledMaxs;
  G4int theNVoxels;
};

#endif
