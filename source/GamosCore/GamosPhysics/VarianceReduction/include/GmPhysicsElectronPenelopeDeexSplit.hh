#ifndef GmPhysicsElectronPenelopeDeexSplit_HH
#define GmPhysicsElectronPenelopeDeexSplit_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronPenelopeDeexSplit : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronPenelopeDeexSplit(const G4String& name = "electron-deexSplit");
  
  virtual ~GmPhysicsElectronPenelopeDeexSplit();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
