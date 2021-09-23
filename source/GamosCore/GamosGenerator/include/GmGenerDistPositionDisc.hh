#ifndef GmGenerDistPositionDisc_HH
#define GmGenerDistPositionDisc_HH

#include "GmVGenerDistPosition2D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPositionDisc : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionDisc();
  virtual ~GmGenerDistPositionDisc(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  G4double GetRadius() const{
    return theRadius;
  }
  void SetRadius( G4double rad ){
    theRadius = rad;
  }
private:
  G4double theRadius;
};

#endif
