#ifndef GmPSeplusAnnihilation_h
#define GmPSeplusAnnihilation_h 1

#include "GmVPSGammaProcess.hh"
#include "G4Positron.hh"
#include "GmVPSEnergyLossProcess.hh"

class GmPSeplusAnnihilation : public GmVPSGammaProcess
{

public:

  GmPSeplusAnnihilation(const G4String& name = "annihil");

  virtual ~GmPSeplusAnnihilation();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& track,
                             const G4Step& stepData);

  virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            );

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  
  G4bool  isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
