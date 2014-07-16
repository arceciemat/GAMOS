#ifndef GmDistributionRatio_h
#define GmDistributionRatio_h 1

#include "GmVStringDistribution.hh"

class GmDistributionRatio : public GmVDistribution
{
  
public: // with description
  GmDistributionRatio(G4String name);
  virtual ~GmDistributionRatio(){};
  virtual void BuildData();

public: // with description
  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);

  virtual void ReadFile() {}; // data is read for each of the two sub distributions
  virtual void Normalize() {};

private:
  GmVDistribution* theDistPre;
  GmVDistribution* theDistPost;

  G4double theMaxValue;
  
  G4bool bNoRepeatWeight;
};

#endif
