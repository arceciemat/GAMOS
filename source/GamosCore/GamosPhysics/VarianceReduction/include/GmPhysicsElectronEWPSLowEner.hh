#ifndef GmPhysicsElectronEWPSLowEner_HH
#define GmPhysicsElectronEWPSLowEner_HH 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmVPhysicsElectron.hh"
#include "globals.hh"

class GmPhysicsElectronEWPSLowEner : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronEWPSLowEner(const G4String& name = "electron-EWPS");
  
  virtual ~GmPhysicsElectronEWPSLowEner();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif
