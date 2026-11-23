#ifndef GmPhysicsPositronPenelope_HH
#define GmPhysicsPositronPenelope_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsPositronPenelope : public G4VPhysicsConstructor {

public: 

  GmPhysicsPositronPenelope(const G4String& name = "positron-penelope", G4int type=111);
  
  virtual ~GmPhysicsPositronPenelope();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
