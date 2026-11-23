#ifndef GmPhysicsPositronNuclear_HH
#define GmPhysicsPositronNuclear_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsPositronNuclear : public G4VPhysicsConstructor {

public: 

  GmPhysicsPositronNuclear(const G4String& name = "electron-nuclear", G4int type=111);
  
  virtual ~GmPhysicsPositronNuclear();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
