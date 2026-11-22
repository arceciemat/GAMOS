#ifndef GmEDepo_HH
#define GmEDepo_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4ParticleDefinition;

class GmEDepo
{

public:
  
  GmEDepo(G4double edepo, G4ThreeVector pos, G4ParticleDefinition* part, G4double tim );
  ~GmEDepo(){};

  G4double GetEnergy() const { return theEnergy; }
  G4ThreeVector GetPosition() const { return thePosition; }

  void SetEnergy( G4double ener ) { theEnergy = ener; }
  void SetPosition( G4ThreeVector pos ) { thePosition = pos; }

  G4double GetTime() const { return theTime; }
  void SetTime( G4double tim ) { theTime = tim; }

  G4ParticleDefinition* GetParticle() const {
    return theParticle; }


  void Print(std::ostream& os = G4cout, G4bool bVerbose = TRUE);

private:
  G4double theEnergy;
  G4ThreeVector thePosition;
  G4ParticleDefinition* theParticle;
  G4double theTime;

};

#endif

