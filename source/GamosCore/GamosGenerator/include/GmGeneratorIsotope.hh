#ifndef GmGeneratorIsotope_HH
#define GmGeneratorIsotope_HH

class G4ParticleGun;
class G4Event;
class GmIsotope;
class G4PrimaryVertex;

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

class GmGeneratorIsotope:public G4VUserPrimaryGeneratorAction
{
public:
  GmGeneratorIsotope();
  ~GmGeneratorIsotope();
  void GeneratePrimaries(G4Event* evt);

private:
  G4double theLastEventTime;
};

#endif
