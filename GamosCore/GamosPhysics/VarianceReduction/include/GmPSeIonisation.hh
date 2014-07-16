#ifndef GmPSeIonisation_h
#define GmPSeIonisation_h 1

#include "GmVPSEnergyLossProcess.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4VEmModel.hh"

class G4Material;
class G4ParticleDefinition;

class GmPSeIonisation : public GmVPSEnergyLossProcess
{

public:

  GmPSeIonisation(const G4String& name = "eIoni");

  virtual ~GmPSeIonisation();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
					   const G4ParticleDefinition*);

  virtual G4double MinPrimaryEnergy(const G4ParticleDefinition*,
				    const G4Material*, G4double cut);

private:

  // hide assignment operator
  GmPSeIonisation & operator=(const GmPSeIonisation &right);
  GmPSeIonisation(const GmPSeIonisation&);

  const G4ParticleDefinition* theElectron;
  const G4ParticleDefinition* particle;

  G4bool isElectron;
  G4bool isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
