#ifndef GmPhysicsPositronStandard_HH
#define GmPhysicsPositronStandard_HH 1

#include "GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsPositronStandard : public GmVPhysicsElectron {

public: 

  GmPhysicsPositronStandard(const G4String& name = "positron-standard", G4int type=111);
  
  virtual ~GmPhysicsPositronStandard();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
