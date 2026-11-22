#ifndef GmNumericDistributionLinLin_h
#define GmNumericDistributionLinLin_h 1

#include "GmVNumericDistribution.hh"

class GmNumericDistributionLinLin : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionLinLin(G4String name);
  virtual ~GmNumericDistributionLinLin(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

};

#endif
