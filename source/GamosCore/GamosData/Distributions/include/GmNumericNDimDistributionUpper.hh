#ifndef GmNumericNDimDistributionUpper_h
#define GmNumericNDimDistributionUpper_h 1

#include "GmVNumericNDimDistribution.hh"

class GmNumericNDimDistributionUpper : public GmVNumericNDimDistribution
{
  
public: // with description
  GmNumericNDimDistributionUpper(G4String name);
  virtual ~GmNumericNDimDistributionUpper(){};
  
public: 
  virtual G4double InterpolateValue( const std::vector<std::vector<G4int> >& indices );

};

#endif
