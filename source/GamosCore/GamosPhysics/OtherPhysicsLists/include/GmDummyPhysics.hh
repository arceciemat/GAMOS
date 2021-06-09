#ifndef GmDummyPhysics_h
#define GmDummyPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class GmDummyPhysics: public G4VModularPhysicsList
{
public:
  GmDummyPhysics();
  virtual ~GmDummyPhysics();

  void ConstructParticles();

  virtual void SetCuts();

  virtual void ConstructProcess();  

private:
};

#endif
