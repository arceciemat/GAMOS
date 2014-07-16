#ifndef GmPhysicsElectronLowEnerDeexSplit_HH
#define GmPhysicsElectronLowEnerDeexSplit_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronLowEnerDeexSplit : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronLowEnerDeexSplit(const G4String& name = "electron-deexSplit");
  
  virtual ~GmPhysicsElectronLowEnerDeexSplit();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
