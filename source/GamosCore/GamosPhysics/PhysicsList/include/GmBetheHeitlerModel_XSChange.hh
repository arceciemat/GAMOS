#ifndef GmBetheHeitlerModel_XSChange_h
#define GmBetheHeitlerModel_XSChange_h 1

#include "G4BetheHeitlerModel.hh"
#include "GmXSChange_DistributionUser.hh"

class GmBetheHeitlerModel_XSChange : public G4BetheHeitlerModel, public GmXSChange_DistributionUser
{

public:

  explicit GmBetheHeitlerModel_XSChange(const G4ParticleDefinition* p = 0, 
                               const G4String& nam = "BetheHeitler");
 
  virtual ~GmBetheHeitlerModel_XSChange();

  virtual G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition*,
                                              G4double kinEnergy, 
                                              G4double Z, 
                                              G4double A=0., 
                                              G4double cut=0.,
                                              G4double emax=DBL_MAX) override;
};


#endif
