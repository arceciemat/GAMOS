#ifndef GmGenerDistTimeConstant_HH
#define GmGenerDistTimeConstant_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"
class GmParticleSource;

class GmGenerDistTimeConstant : public GmVGenerDistTime
{
public:
  GmGenerDistTimeConstant();
  virtual ~GmGenerDistTimeConstant(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  G4double GetTime() const { return theTime; }
  void SetTime( G4double tim ){ theTime = tim; }

  virtual void SetParams( const std::vector<G4String>& params );

private:

  G4double theTime;

};

#endif
