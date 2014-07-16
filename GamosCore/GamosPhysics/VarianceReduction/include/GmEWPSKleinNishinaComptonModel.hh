#ifndef GmEWPSKleinNishinaComptonModel_h
#define GmEWPSKleinNishinaComptonModel_h 1

#include "G4KleinNishinaCompton.hh"
#include "GmVPSModelHelper.hh"

class G4ParticleChangeForGamma;

class GmEWPSKleinNishinaComptonModel : public G4KleinNishinaCompton, public GmVPSModelHelper
{

public:

  GmEWPSKleinNishinaComptonModel(const G4ParticleDefinition* p = 0, 
			const G4String& nam = "GmEWPSKleinNishinaCompton");

  virtual ~GmEWPSKleinNishinaComptonModel();

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
