#ifndef GmPhysicsRadioactiveDecay_h
#define GmPhysicsRadioactiveDecay_h 1

#include "globals.hh"

class G4RadioactiveDecayBase;

class GmPhysicsRadioactiveDecay
{
public: 
  GmPhysicsRadioactiveDecay();
  ~GmPhysicsRadioactiveDecay();

public: 
  virtual void ConstructParticle(){};
  virtual void ConstructProcess();

private:
  G4RadioactiveDecayBase*  theRadioactiveDecay;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
