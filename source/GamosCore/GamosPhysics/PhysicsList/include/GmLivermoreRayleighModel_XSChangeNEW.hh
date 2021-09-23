#ifndef GmLivermoreRayleighModel_XSChangeNEW_h
#define GmLivermoreRayleighModel_XSChangeNEW_h 1

#include "G4LivermoreRayleighModel.hh"
#include "GmXSChange_DistributionUser.hh"

class GmLivermoreRayleighModel_XSChangeNEW : public G4LivermoreRayleighModel, public GmXSChange_DistributionUser
{

public:

  GmLivermoreRayleighModel_XSChangeNEW();

  virtual ~GmLivermoreRayleighModel_XSChangeNEW();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  virtual void InitialiseLocal(const G4ParticleDefinition*, 
			       G4VEmModel* masterModel);

  virtual void InitialiseForElement(const G4ParticleDefinition*, G4int Z);

  virtual G4double ComputeCrossSectionPerAtom(
                                const G4ParticleDefinition*,
                                      G4double kinEnergy, 
                                      G4double Z, 
                                      G4double A=0, 
                                      G4double cut=0,
                                      G4double emax=DBL_MAX);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy);

  inline void SetLowEnergyThreshold(G4double);

private:

  void ReadData(size_t Z, const char* path = 0);

  GmLivermoreRayleighModel_XSChangeNEW & operator=(const GmLivermoreRayleighModel_XSChangeNEW &right);
  GmLivermoreRayleighModel_XSChangeNEW(const GmLivermoreRayleighModel_XSChangeNEW&);

  G4bool isInitialised;
  G4int verboseLevel;

  G4double lowEnergyLimit;  

  static G4int maxZ;
  static G4LPhysicsFreeVector* dataCS[101];

  G4ParticleChangeForGamma* fParticleChange;

};

inline void GmLivermoreRayleighModel_XSChangeNEW::SetLowEnergyThreshold(G4double val)
{
  lowEnergyLimit = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
