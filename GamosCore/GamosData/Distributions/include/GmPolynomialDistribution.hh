#ifndef GmPolynomialDistribution_h
#define GmPolynomialDistribution_h 1

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"
#include <vector>

class GmPolynomialDistribution : public GmVDistribution
{
  
public: 
  GmPolynomialDistribution(G4String name);
  virtual ~GmPolynomialDistribution(){};

  virtual void BuildData();
  virtual void ReadFile(){}

  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetNumericValueFromIndex(const G4double indexVal);

  virtual void SetParameters( std::vector<G4String>& );

  virtual void Normalize();

protected:
  
  std::vector<G4double> theParams;

};

#endif
