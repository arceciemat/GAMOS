//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GmXSChange_DistributionUser_h
#define GmXSChange_DistributionUser_h 1

#include "G4PhotoElectricEffect.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmVNumericDistribution;

class GmXSChange_DistributionUser 
{
public:  // with description

  explicit GmXSChange_DistributionUser(const G4String&);

  virtual ~GmXSChange_DistributionUser(){};

  void BuildDistribution(G4String name);

  G4double GetDistributionValue(G4double energy);

protected:
  GmVNumericDistribution* theDistribution;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

