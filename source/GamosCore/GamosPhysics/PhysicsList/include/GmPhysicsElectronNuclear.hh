#ifndef GmPhysicsElectronNuclear_HH
#define GmPhysicsElectronNuclear_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsElectronNuclear : public G4VPhysicsConstructor {

public: 

  GmPhysicsElectronNuclear(const G4String& name = "electron-nuclear", G4int type=111);
  
  virtual ~GmPhysicsElectronNuclear();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
