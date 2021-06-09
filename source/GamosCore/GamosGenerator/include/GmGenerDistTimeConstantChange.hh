#ifndef GmGenerDistTimeConstantChange_HH
#define GmGenerDistTimeConstantChange_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"
class GmParticleSource;

class GmGenerDistTimeConstantChange : public GmVGenerDistTime
{
public:
  GmGenerDistTimeConstantChange();
  virtual ~GmGenerDistTimeConstantChange(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  G4double GetInterval() const { return theInterval; }
  void SetInterval( G4double tim ){ theInterval = tim; }

  virtual void SetParams( const std::vector<G4String>& params );

private:

  G4double thePreviousTime;
  G4double theInterval;

};

#endif
