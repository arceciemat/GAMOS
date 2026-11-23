#ifndef GmNumericDistributionLogLog_h
#define GmNumericDistributionLogLog_h 1

#include "GmVNumericDistribution.hh"

class GmNumericDistributionLogLog : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionLogLog(G4String name);
  virtual ~GmNumericDistributionLogLog(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

};

#endif
