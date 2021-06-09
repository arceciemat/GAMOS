#ifndef GmUBSLivermoreBremsstrahlungModel_HH
#define GmUBSLivermoreBremsstrahlungModel_HH 1

#define private protected
#include "G4LivermoreBremsstrahlungModel.hh"
#include "GmVPSModelHelper.hh"
#include "G4ThreeVector.hh"

class GmUBSLivermoreBremsstrahlungModel : public G4LivermoreBremsstrahlungModel, GmVPSModelHelper
{

public:
  
  GmUBSLivermoreBremsstrahlungModel(const G4ParticleDefinition* p=0,
			 const G4String& processName = "GmEWPSLowEnBrem");
  
  virtual ~GmUBSLivermoreBremsstrahlungModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   G4ThreeVector position,
				   G4double wgt,
				   const G4Track& track);
protected:
};

#endif
