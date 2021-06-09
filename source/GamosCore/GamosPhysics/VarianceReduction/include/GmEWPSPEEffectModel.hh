
#ifndef GmEWPSPEEffectModel_h
#define GmEWPSPEEffectModel_h 1

#include "G4PEEffectModel.hh"
#include "GmVPSModelHelper.hh"

class GmEWPSPEEffectModel : public G4PEEffectModel, public GmVPSModelHelper
{

public:

  GmEWPSPEEffectModel(const G4ParticleDefinition* p = 0,
		  const G4String& nam = "EWPSPhotoElectric");

  virtual ~GmEWPSPEEffectModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
