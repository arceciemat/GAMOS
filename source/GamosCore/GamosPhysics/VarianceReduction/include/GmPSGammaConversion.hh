#ifndef GmPSGammaConversion_h
#define GmPSGammaConversion_h 1

#include "globals.hh"
#include "GmVPSGammaProcess.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ParticleDefinition;
class G4VEmModel;
class G4MaterialCutsCouple;
class G4DynamicParticle;

class GmPSGammaConversion : public GmVPSGammaProcess

{
public:  // with description

  GmPSGammaConversion(const G4String& processName ="conv",
		      G4ProcessType type = fElectromagnetic);

  virtual ~GmPSGammaConversion();

  // true for Gamma only.
  virtual G4bool IsApplicable(const G4ParticleDefinition&);

  // Print few lines of informations about the process: validity range,
  virtual void PrintInfo();

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
     


  G4bool  isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
#endif
 
