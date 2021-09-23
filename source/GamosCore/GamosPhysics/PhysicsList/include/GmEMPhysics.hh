// ----------------------------------------------------------------------------
#ifndef GmEMPhysics_h
#define GmEMPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class GmEMPhysicsMessenger;

class GmEMPhysics: public G4VModularPhysicsList
{
public:
  GmEMPhysics();
  virtual ~GmEMPhysics();

  virtual void SetCuts();
  virtual G4bool ReplacePhysicsList(const G4String& name);  

  virtual void ConstructParticles();  

  virtual void ConstructProcess();  

protected:
  G4bool electronIsRegistered;
  G4bool positronIsRegistered;
  G4bool gammaIsRegistered;

  GmEMPhysicsMessenger* messenger;
};

#endif
