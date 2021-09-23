#ifndef GmVPhysicsElectron_HH
#define GmVPhysicsElectron_HH 1

#include "G4VPhysicsConstructor.hh"
#include "G4ParticleTable.hh"
class G4VEnergyLossProcess;
class G4LivermoreIonisationModel;
class G4LivermoreBremsstrahlungModel;
class G4PenelopeBremsstrahlungModel;
class G4SeltzerBergerModel;


class GmVPhysicsElectron : public G4VPhysicsConstructor {

public: 

  GmVPhysicsElectron(const G4String& name, G4int type);
  
  virtual ~GmVPhysicsElectron();
  
  // This method is dummy for GamosEMLowEnPhysics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess() = 0;

protected:
  void SelectBremssAngularDist(G4SeltzerBergerModel* bremsModel, G4String part = "Electron" );

  void SelectBremssAngularDist(G4LivermoreBremsstrahlungModel* bremsModel, G4String part = "Electron" );

  void AddDeexcitation();

};

#endif
