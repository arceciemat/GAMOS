#ifndef GmRayleighScattering_XSChange_h
#define GmRayleighScattering_XSChange_h 1

#include "G4RayleighScattering.hh"
#include "GmXSChange_DistributionUser.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmRayleighScattering_XSChange : public G4RayleighScattering, public GmXSChange_DistributionUser

{
public:  // with description

  explicit GmRayleighScattering_XSChange(const G4String& processName ="Rayl",
		                G4ProcessType type = fElectromagnetic);

  virtual ~GmRayleighScattering_XSChange();

  // print description in html
  virtual void ProcessDescription(std::ostream&) const override;

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*) override;

private:
     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
  
#endif
 
