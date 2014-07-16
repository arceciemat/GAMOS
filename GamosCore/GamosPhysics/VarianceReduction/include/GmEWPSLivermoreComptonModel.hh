#ifndef GmEWPSLivermoreComptonModel_h
#define GmEWPSLivermoreComptonModel_h 1

#include "G4LivermoreComptonModel.hh"
#include "GmVPSModelHelper.hh"
#include "G4Track.hh"

class GmEWPSLivermoreComptonModel : public G4LivermoreComptonModel, public GmVPSModelHelper
{

public:

  GmEWPSLivermoreComptonModel(const G4ParticleDefinition* p = 0, 
		          const G4String& nam = "GmEWPSLivermoreCompton");

  virtual ~GmEWPSLivermoreComptonModel();

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
