#ifndef GmGenerDistPositionPETImage_HH
#define GmGenerDistPositionPETImage_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;
class G4PhantomParameterisation;
#include "G4AffineTransform.hh"
#include <set>


class GmGenerDistPositionPETImage : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionPETImage();
  virtual ~GmGenerDistPositionPETImage(){};

  virtual void SetParams( const std::vector<G4String>& params );

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );


private:
  void Initialise();
  
  G4String theFileName;
  G4double theMinX, theMinY, theMinZ;
  G4double theVoxelDimX, theVoxelDimY, theVoxelDimZ;
  unsigned int theNVoxelX, theNVoxelY, theNVoxelZ;
  std::multiset<G4double> theProb;
  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;
};

#endif
