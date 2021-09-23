#ifndef GmGenerDistTimeConstantChangeRandom_HH
#define GmGenerDistTimeConstantChangeRandom_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"
class GmParticleSource;

class GmGenerDistTimeConstantChangeRandom : public GmVGenerDistTime
{
public:
  GmGenerDistTimeConstantChangeRandom();
  virtual ~GmGenerDistTimeConstantChangeRandom(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  G4double GetInterval() const {
    return theInterval;
  }
  void SetInterval( G4double tim ){
    theInterval = tim;
    //    G4cout << " GmGenerDistTimeConstantChangeRandom SetInterval " << theInterval << G4endl; //GDEB
  }

  virtual void SetParams( const std::vector<G4String>& params );

private:

  G4double thePreviousTime;
  G4double theInterval;

};

#endif
