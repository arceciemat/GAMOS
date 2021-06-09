#ifndef GmPhysicsElectronLowEnerZBSDeexSplit_HH
#define GmPhysicsElectronLowEnerZBSDeexSplit_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronLowEnerZBSDeexSplit : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronLowEnerZBSDeexSplit(const G4String& name = "electron-deexSplit");
  
  virtual ~GmPhysicsElectronLowEnerZBSDeexSplit();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
