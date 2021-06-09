#ifndef GmEWPSBremsstrahlungModel_h
#define GmEWPSBremsstrahlungModel_h 1

#include "G4eBremsstrahlungModel.hh"
#include "GmVPSModelHelper.hh"

class G4Element;
class G4ParticleChangeForLoss;

class GmEWPSBremsstrahlungModel : public G4eBremsstrahlungModel, public GmVPSModelHelper
{

public:

  GmEWPSBremsstrahlungModel(const G4ParticleDefinition* p = 0, 
			 const G4String& nam = "eBrem");

  virtual ~GmEWPSBremsstrahlungModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);

};

#endif
