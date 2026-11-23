#ifndef GmPhysicsXrayRefraction_HH
#define GmPhysicsXrayRefraction_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GmPhysicsXrayRefraction {

public: 

  GmPhysicsXrayRefraction();
  
  ~GmPhysicsXrayRefraction();
  
  virtual void ConstructProcess();
};

#endif
