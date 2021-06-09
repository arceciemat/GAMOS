#ifndef GmEWPSBetheHeitlerModel_h
#define GmEWPSBetheHeitlerModel_h 1

#include "GmVPSModelHelper.hh"
#include "G4PhysicsTable.hh"
#include "G4BetheHeitlerModel.hh"

class G4ParticleChangeForGamma;

class GmEWPSBetheHeitlerModel : public G4BetheHeitlerModel, public GmVPSModelHelper
{

public:

  GmEWPSBetheHeitlerModel(const G4ParticleDefinition* p = 0, 
		      const G4String& nam = "Bethe-Heitler");
 
  virtual ~GmEWPSBetheHeitlerModel();
  
  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
                                   const G4Track& track);
  
};

#endif
