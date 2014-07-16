#ifndef GmGenerDistTimeDecay_HH
#define GmGenerDistTimeDecay_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"

class GmParticleSource;
class GmGenerator;

class GmGenerDistTimeDecay : public GmVGenerDistTime
{
public:
  GmGenerDistTimeDecay();
  virtual ~GmGenerDistTimeDecay(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  void SetActivity( const G4double act );

  static G4double GetCurrentActivity() {
    return theCurrentActivity;
  }

private:
  static G4double theCurrentActivity;

protected:
  G4double theActivity;
  G4double theLifeTime;
  GmGenerator* theGenerator;
};

#endif
