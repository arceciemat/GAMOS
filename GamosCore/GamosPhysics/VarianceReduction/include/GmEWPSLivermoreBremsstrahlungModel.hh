#ifndef GmEWPSLivermoreBremsstrahlungModel_HH
#define GmEWPSLivermoreBremsstrahlungModel_HH 1

#include "GmVPSModelHelper.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4ThreeVector.hh"

class GmEWPSLivermoreBremsstrahlungModel : public G4LivermoreBremsstrahlungModel, public GmVPSModelHelper
{

public:
  
  GmEWPSLivermoreBremsstrahlungModel(const G4ParticleDefinition* p=0,
			 const G4String& processName = "GmEWPSLowEnBrem");
  
  virtual ~GmEWPSLivermoreBremsstrahlungModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);

};

#endif
