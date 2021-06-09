#ifndef GmPhysicsElectronUBSLowEner_HH
#define GmPhysicsElectronUBSLowEner_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronUBSLowEner : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronUBSLowEner(const G4String& name = "electron-UBS");
  
  virtual ~GmPhysicsElectronUBSLowEner();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
