#ifndef GmPhysicsDecay_HH
#define GmPhysicsDecay_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "G4Decay.hh"

class GmPhysicsDecay 
{
public: 
  
  GmPhysicsDecay();
  
  ~GmPhysicsDecay();
  
  virtual void ConstructProcess();  
};
#endif
