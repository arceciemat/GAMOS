#ifndef GmGenerDistTimeDecayWithProd_HH
#define GmGenerDistTimeDecayWithProd_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"

class GmParticleSource;
class GmGenerator;

class GmGenerDistTimeDecayWithProd : public GmVGenerDistTime
{
public:
  GmGenerDistTimeDecayWithProd();
  virtual ~GmGenerDistTimeDecayWithProd(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  void SetProduction( const G4double act );
  void SetLifeTime( const G4double act );
  void SetPreviousTime( const G4double act );
  
  G4double GetProduction() {
    return theProduction;
  }

protected:
  G4double theProduction;
  G4double theProductionInv;
  G4double thePreviousTime;
  G4double theLifeTime;
  GmGenerator* theGenerator;
};

#endif
