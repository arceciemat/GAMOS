#ifndef GmPSeBremsstrahlung_h
#define GmPSeBremsstrahlung_h 1

#include "GmVPSEnergyLossProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

class GmPSeBremsstrahlung : public GmVPSEnergyLossProcess
{

public:

  GmPSeBremsstrahlung(const G4String& name = "eBrem");

  virtual ~GmPSeBremsstrahlung(){};

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
					   const G4ParticleDefinition*);

  const G4ParticleDefinition* particle;
  G4bool   isInitialised;

private:

  // hide assignment operator
  GmPSeBremsstrahlung & operator=(const GmPSeBremsstrahlung &right);
  GmPSeBremsstrahlung(const GmPSeBremsstrahlung&);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
