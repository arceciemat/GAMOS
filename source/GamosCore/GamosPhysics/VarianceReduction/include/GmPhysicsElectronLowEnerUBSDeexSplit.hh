#ifndef GmPhysicsElectronLowEnerUBSDeexSplit_HH
#define GmPhysicsElectronLowEnerUBSDeexSplit_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronLowEnerUBSDeexSplit : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronLowEnerUBSDeexSplit(const G4String& name = "electron-deexSplit");
  
  virtual ~GmPhysicsElectronLowEnerUBSDeexSplit();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
