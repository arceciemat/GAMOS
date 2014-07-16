#ifndef GmPhysicsMultipleScattering_hh
#define GmPhysicsMultipleScattering_hh 1

#include "G4VMultipleScattering.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmPhysicsMultipleScattering : public G4VMultipleScattering

{
public:    // with description

  GmPhysicsMultipleScattering(const G4String& processName, const G4String particleName);

  virtual ~GmPhysicsMultipleScattering();

  // returns true for charged particles, false otherwise
  G4bool IsApplicable (const G4ParticleDefinition& p);

  // Print few lines of informations about the process: validity range,
  void PrintInfo();

  // geom. step length distribution should be sampled or not
  void Setsamplez(G4bool value) { samplez = value;};

  // to reduce the energy/step dependence
  void Setdtrl(G4double value) { dtrl = value;};

  // 'soften' step limitation above lambdalimit
  void SetLambdalimit(G4double value) { lambdalimit = value;};

protected:

  // This function initialise models
  void InitialiseProcess(const G4ParticleDefinition*);

private:        // data members

  G4double lambdalimit;
  G4double dtrl;

  G4bool   samplez;
  G4bool   isInitialized;
  
  G4String theParticleName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
