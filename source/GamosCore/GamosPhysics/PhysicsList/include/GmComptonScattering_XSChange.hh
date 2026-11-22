#ifndef GmComptonScattering_XSChange_h
#define GmComptonScattering_XSChange_h 1

#include "G4ComptonScattering.hh"
#include "GmXSChange_DistributionUser.hh"

class GmComptonScattering_XSChange : public G4ComptonScattering, public GmXSChange_DistributionUser
{
public:  // with description

  explicit GmComptonScattering_XSChange(const G4String& processName ="compt",
			       G4ProcessType type = fElectromagnetic);

  virtual ~GmComptonScattering_XSChange();

  // print description in html
  virtual void ProcessDescription(std::ostream&) const override;

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*) override;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
#endif
 
