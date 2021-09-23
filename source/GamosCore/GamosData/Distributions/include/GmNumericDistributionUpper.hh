#ifndef GmNumericDistributionUpper_h
#define GmNumericDistributionUpper_h 1

#include "GmVNumericDistribution.hh"
class TH1F;

class GmNumericDistributionUpper : public GmVNumericDistribution
{
  
public: // with description
  GmNumericDistributionUpper(G4String name);
  virtual ~GmNumericDistributionUpper(){};
  
public: 
  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp );

  virtual G4double GetMinimum(TH1F*);

};

#endif
