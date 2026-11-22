#ifndef GmVPhysicsGamma_HH
#define GmVPhysicsGamma_HH 1

#include "G4ParticleTable.hh"
#include "G4VPhysicsConstructor.hh"

class G4VEmProcess;
class G4LivermorePhotoElectricModel;

class GmVPhysicsGamma : public G4VPhysicsConstructor {

public: 

  GmVPhysicsGamma(const G4String& name, G4int type);
  
  virtual ~GmVPhysicsGamma();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess() = 0;

  void AddDeexcitation();

};

#endif
