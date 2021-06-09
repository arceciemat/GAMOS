#ifndef GmGenerDistPositionInVoxelsFromFile_HH
#define GmGenerDistPositionInVoxelsFromFile_HH

#include "GmVGenerDistPosition.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include <map>

class GmParticleSource;
typedef std::map<G4double,G4int> mdi;

class GmGenerDistPositionInVoxelsFromFile : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInVoxelsFromFile(){};
  virtual ~GmGenerDistPositionInVoxelsFromFile(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:

  void ErrorTooShortFile();

  mdi theProbAccum;
  G4double theProbSum;
  G4int theNVoxelX;
  G4int theNVoxelY;
  G4int theNVoxelZ;
  G4int theNVoxelXY;
  G4double theOffsetX;
  G4double theOffsetY;
  G4double theOffsetZ;
  G4double theDimX;
  G4double theDimY;
  G4double theDimZ;

  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;

};

#endif
