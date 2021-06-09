#ifndef GmEWPSLivermorePhotoElectricModel_h
#define GmEWPSLivermorePhotoElectricModel_h 1

#include "G4LivermorePhotoElectricModel.hh"
#include "GmVPSModelHelper.hh"

class GmEWPSLivermorePhotoElectricModel : public G4LivermorePhotoElectricModel, public GmVPSModelHelper
{

public:

  GmEWPSLivermorePhotoElectricModel(const G4ParticleDefinition* p = 0, 
		     const G4String& nam = "EWPSLivermorePhElectric");

  virtual ~GmEWPSLivermorePhotoElectricModel();

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
