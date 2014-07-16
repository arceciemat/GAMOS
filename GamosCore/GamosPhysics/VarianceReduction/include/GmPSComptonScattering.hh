#ifndef GmPSComptonScattering_h
#define GmPSComptonScattering_h 1

#include "globals.hh"
#include "GmVPSGammaProcess.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4ParticleDefinition;
class G4VEmModel;
class G4MaterialCutsCouple;
class G4DynamicParticle;

class GmPSComptonScattering : public GmVPSGammaProcess
{
public:  // with description

  GmPSComptonScattering(const G4String& processName ="compt",
		      G4ProcessType type = fElectromagnetic);

  virtual ~GmPSComptonScattering();

  
protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
     
  G4bool       isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
#endif
 
