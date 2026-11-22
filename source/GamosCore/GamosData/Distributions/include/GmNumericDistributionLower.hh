#ifndef GmNumericDistributionLower_h
#define GmNumericDistributionLower_h 1

#include "GmVNumericDistribution.hh"

class GmNumericDistributionLower : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionLower(G4String name);
  virtual ~GmNumericDistributionLower(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

};

#endif
