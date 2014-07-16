#ifndef GmStringDistribution_h
#define GmStringDistribution_h 1

#include "GmVStringDistribution.hh"

class GmStringDistribution : public GmVStringDistribution
{
  
public: // with description
  GmStringDistribution(G4String name);
  virtual ~GmStringDistribution(){};

public: // with description
  virtual G4double GetValueFromStep(const G4Step* aStep);

private:
};

#endif
