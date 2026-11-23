#ifndef GmGammaConversion_XSChange_h
#define GmGammaConversion_XSChange_h 1

#include "G4GammaConversion.hh"
#include "GmXSChange_DistributionUser.hh"

class GmGammaConversion_XSChange : public G4GammaConversion, public GmXSChange_DistributionUser

{
public:  // with description

  explicit GmGammaConversion_XSChange(const G4String& processName ="conv",
			     G4ProcessType type = fElectromagnetic);

  virtual ~GmGammaConversion_XSChange();


  // print documentation in html format
  virtual void ProcessDescription(std::ostream&) const override;

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*) override;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
#endif
 
