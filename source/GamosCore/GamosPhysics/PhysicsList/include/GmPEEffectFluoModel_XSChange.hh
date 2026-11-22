// -------------------------------------------------------------------
//

#ifndef GmPEEffectFluoModel_XSChange_h
#define GmPEEffectFluoModel_XSChange_h 1

#include "G4PEEffectFluoModel.hh"
#include "GmXSChange_DistributionUser.hh"
#include <vector>

class G4ParticleChangeForGamma;
class G4VAtomDeexcitation;
class GmVNumericDistribution;

class GmPEEffectFluoModel_XSChange : public G4PEEffectFluoModel, public GmXSChange_DistributionUser
{

public:

  explicit GmPEEffectFluoModel_XSChange(const G4String& nam = "PhotoElectric");

  virtual ~GmPEEffectFluoModel_XSChange();

  virtual 
  G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
				      G4double kinEnergy,
				      G4double Z,
				      G4double A,
				      G4double, G4double) override;

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
