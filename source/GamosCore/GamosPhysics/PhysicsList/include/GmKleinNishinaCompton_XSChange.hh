#ifndef GmKleinNishinaCompton_XSChange_h
#define GmKleinNishinaCompton_XSChange_h 1

#include "G4KleinNishinaCompton.hh"
#include "GmXSChange_DistributionUser.hh"

class GmKleinNishinaCompton_XSChange : public G4KleinNishinaCompton, public GmXSChange_DistributionUser
{

public:

  explicit GmKleinNishinaCompton_XSChange(const G4ParticleDefinition* p = nullptr, 
				 const G4String& nam = "Klein-Nishina");

  virtual ~GmKleinNishinaCompton_XSChange();

  virtual G4double ComputeCrossSectionPerAtom(
                                const G4ParticleDefinition*,
                                      G4double kinEnergy, 
                                      G4double Z, 
                                      G4double A, 
                                      G4double cut,
                                      G4double emax) override;
	      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
