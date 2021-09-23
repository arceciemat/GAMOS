#ifndef GmDummyPhysics_h
#define GmDummyPhysics_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class GmDummyPhysics: public G4VUserPhysicsList
{
public:
  GmDummyPhysics();
  ~GmDummyPhysics();

  void ConstructParticle();

  void SetCuts();

  void ConstructProcess();  

private:
};

#endif
