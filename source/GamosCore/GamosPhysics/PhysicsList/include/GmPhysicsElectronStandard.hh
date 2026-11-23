#ifndef GmPhysicsElectronStandard_HH
#define GmPhysicsElectronStandard_HH 1

#include "GmVPhysicsElectron.hh"
class G4eBremsstrahlungModel;

class GmPhysicsElectronStandard : public GmVPhysicsElectron {

public: 

  GmPhysicsElectronStandard(const G4String& name = "electron-standard", G4int type=111);
  
  virtual ~GmPhysicsElectronStandard();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();

};

#endif
