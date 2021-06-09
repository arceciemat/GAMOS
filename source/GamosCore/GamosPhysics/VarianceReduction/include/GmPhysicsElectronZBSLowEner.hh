#ifndef GmPhysicsElectronZBSLowEner_HH
#define GmPhysicsElectronZBSLowEner_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronZBSLowEner : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronZBSLowEner(const G4String& name = "electron-ZBS");
  
  virtual ~GmPhysicsElectronZBSLowEner();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
