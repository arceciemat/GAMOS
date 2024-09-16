#ifndef GmPhysicsCoulombScattering_HH
#define GmPhysicsCoulombScattering_HH 1

#include "globals.hh"

class GmPhysicsCoulombScattering {

public: 

  GmPhysicsCoulombScattering( G4double lowEnergyLimit = 0. );
  
  ~GmPhysicsCoulombScattering();
  
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4double theLowEnergyLimit;
};

#endif
