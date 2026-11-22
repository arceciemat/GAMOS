#ifndef GmLivermoreRayleighModel_XSChange_h
#define GmLivermoreRayleighModel_XSChange_h 1

#include "G4LivermoreRayleighModel.hh"
#include "GmXSChange_DistributionUser.hh"

class GmLivermoreRayleighModel_XSChange : public G4LivermoreRayleighModel, public GmXSChange_DistributionUser
{

public:

  GmLivermoreRayleighModel_XSChange();

  virtual ~GmLivermoreRayleighModel_XSChange();

  virtual G4double ComputeCrossSectionPerAtom(
                                const G4ParticleDefinition*,
                                      G4double kinEnergy, 
                                      G4double Z, 
                                      G4double A=0, 
                                      G4double cut=0,
                                      G4double emax=DBL_MAX);

};

#endif
