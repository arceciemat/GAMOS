#ifndef GmPhysicsElectronPenelope_HH
#define GmPhysicsElectronPenelope_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsElectronPenelope : public G4VPhysicsConstructor {

public: 

  GmPhysicsElectronPenelope(const G4String& name = "electron-penelope", G4int type=111);
  
  virtual ~GmPhysicsElectronPenelope();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
