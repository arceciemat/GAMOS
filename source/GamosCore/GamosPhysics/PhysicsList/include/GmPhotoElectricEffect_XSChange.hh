//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GmPhotoElectricEffect_XSChange_h
#define GmPhotoElectricEffect_XSChange_h 1

#include "G4PhotoElectricEffect.hh"
#include "GmXSChange_DistributionUser.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmPhotoElectricEffect_XSChange : public G4PhotoElectricEffect, public GmXSChange_DistributionUser
{
public:  // with description

  explicit GmPhotoElectricEffect_XSChange(const G4String& processName ="phot",
				 G4ProcessType type = fElectromagnetic);

  virtual ~GmPhotoElectricEffect_XSChange();

  // print documentation in html format
  virtual void ProcessDescription(std::ostream&) const override;

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*) override;

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

