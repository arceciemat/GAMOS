#ifndef GmEWPSeeToTwoGammaModel_h
#define GmEWPSeeToTwoGammaModel_h 1

#include "G4eeToTwoGammaModel.hh"
#include "GmVPSModelHelper.hh"

class G4ParticleChangeForGamma;

class GmEWPSeeToTwoGammaModel : public G4eeToTwoGammaModel, public GmVPSModelHelper
{

public:

  GmEWPSeeToTwoGammaModel(const G4ParticleDefinition* p = 0,
                      const G4String& nam = "GmEWPSAnnihil");

  virtual ~GmEWPSeeToTwoGammaModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
