#ifndef GmGenerEnergyDistBetaDecay_HH
#define GmGenerEnergyDistBetaDecay_HH

#include "GmVGenerEnergyDist.hh"
#include "globals.hh"
#include <vector>

class GmGenerEnergyDistBetaDecay : public GamosVGenerEnergyDist
{
public:
  GmGenerEnergyDistBetaDecay( const G4String& iso );
  virtual ~GmGenerEnergyDistBetaDecay(){};

  virtual G4double Generate( G4double activity );

private:
  G4String theIsotope;
  std::vector<float> theProb;
  float theProbMax;
};

#endif
