#ifndef GmPhysicsElectronLowEner_HH
#define GmPhysicsElectronLowEner_HH 1

#include "GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronLowEner : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronLowEner(const G4String& name = "electron-lowener", G4int type=111);
  
  virtual ~GmPhysicsElectronLowEner();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();

};

#endif
