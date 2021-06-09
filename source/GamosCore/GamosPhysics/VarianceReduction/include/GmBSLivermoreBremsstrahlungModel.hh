#ifndef GmBSLivermoreBremsstrahlungModel_HH
#define GmBSLivermoreBremsstrahlungModel_HH 1

#include "GmVPSModelHelper.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4ThreeVector.hh"
class GmAnalysisMgr;

class GmBSLivermoreBremsstrahlungModel : public G4LivermoreBremsstrahlungModel, public GmVPSModelHelper
{

public:
  
  GmBSLivermoreBremsstrahlungModel(const G4ParticleDefinition* p=0,
			 const G4String& processName = "GmEWPSLowEnBrem");
  
  virtual ~GmBSLivermoreBremsstrahlungModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);


  GmAnalysisMgr* theAnaMgr;
};

#endif
