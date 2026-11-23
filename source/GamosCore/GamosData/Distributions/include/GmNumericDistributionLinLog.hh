#ifndef GmNumericDistributionLinLog_h
#define GmNumericDistributionLinLog_h 1

#include "GmVNumericDistribution.hh"

class GmNumericDistributionLinLog : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionLinLog(G4String name);
  virtual ~GmNumericDistributionLinLog(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

};

#endif
