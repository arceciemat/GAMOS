#ifndef GmPhysicsOpticalPhoton_HH
#define GmPhysicsOpticalPhoton_HH 1
#include "globals.hh"

class GmPhysicsOpticalPhoton {

public: 

  GmPhysicsOpticalPhoton();
  
  ~GmPhysicsOpticalPhoton();
  
  virtual void ConstructParticle();
  virtual void ConstructProcess();
};

#endif
