#ifndef GmGeometricalBiasingDistribution_h
#define GmGeometricalBiasingDistribution_h 1

#include "GmVStringDistribution.hh"

class GmGeometricalBiasingDistribution : public GmVStringDistribution
{
  
public: // with description
  GmGeometricalBiasingDistribution(G4String name);
  virtual ~GmGeometricalBiasingDistribution(){};
  virtual void BuildData();

public: // with description
  virtual G4double GetValueFromStep(const G4Step* aStep);

private:
  GmVData* theDataPre;
  GmVData* theDataPost;

  G4double theMaxValue;

  G4bool bNoRepeatWeight;
};

#endif
