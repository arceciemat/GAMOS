
#ifndef GmEMExtendedPhysics_h
#define GmEMExtendedPhysics_h 1

#include "GmEMPhysics.hh"
#include "globals.hh"

class GmEMExtendedPhysics: public GmEMPhysics
{
public:
  GmEMExtendedPhysics();
  virtual ~GmEMExtendedPhysics();

  virtual void ConstructParticles();  

  virtual void ConstructProcess();  

private:
  G4bool ionIsRegistered;
  G4bool protonHadronicIsRegistered;
  G4bool chargedParticleIsRegistered;
  G4bool muonIsRegistered;

};

#endif
