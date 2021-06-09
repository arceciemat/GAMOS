#ifndef GmGaussianDistribution_h
#define GmGaussianDistribution_h 1

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

class GmGaussianDistribution : public GmVDistribution
{
  
public: 
  GmGaussianDistribution(G4String name);
  virtual ~GmGaussianDistribution(){};

  virtual void BuildData();
  virtual void ReadFile(){}

  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetNumericValueFromIndex(const G4double indexVal);

  virtual void SetParameters( std::vector<G4String>& );

  virtual void Normalize();

protected:
  
  G4double theSigma2;
  G4double theConstant;
  G4bool bNormalized;

};

#endif
