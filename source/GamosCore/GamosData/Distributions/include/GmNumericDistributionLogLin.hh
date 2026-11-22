#ifndef GmNumericDistributionLogLin_h
#define GmNumericDistributionLogLin_h 1

#include "GmVNumericDistribution.hh"

class GmNumericDistributionLogLin : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionLogLin(G4String name);
  virtual ~GmNumericDistributionLogLin(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

};

#endif
