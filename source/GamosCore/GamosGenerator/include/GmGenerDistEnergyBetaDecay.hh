#ifndef GmGenerDistEnergyBetaDecay_HH
#define GmGenerDistEnergyBetaDecay_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
#include "globals.hh"
#include <vector>
class GmParticleSource;

class GmGenerDistEnergyBetaDecay : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyBetaDecay();
  virtual ~GmGenerDistEnergyBetaDecay(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

private:
  void ReadFile( const G4String filename );

private:
  G4String theIsotope;
  std::vector<float> theProb;
  float theProbMax;
  G4bool bFileRead;
};

#endif
