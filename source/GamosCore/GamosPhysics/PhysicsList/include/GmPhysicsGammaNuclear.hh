#ifndef GmPhysicsGammaNuclear_HH
#define GmPhysicsGammaNuclear_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsGammaNuclear : public G4VPhysicsConstructor {

public: 

  GmPhysicsGammaNuclear(const G4String& name = "gamma-nuclear", G4int type=22);
  
  virtual ~GmPhysicsGammaNuclear();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
