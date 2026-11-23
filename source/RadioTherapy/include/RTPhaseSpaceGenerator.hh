#ifndef __RTPhaseSpaceGenerator__
#define __RTPhaseSpaceGenerator__

#include "G4VUserPrimaryGeneratorAction.hh"

#include <iostream>
#include <vector>
class iaea_header_type;
class iaea_record_type;
class G4ParticleGun;

class RTPhaseSpaceGenerator : public G4VUserPrimaryGeneratorAction
{
public:
  RTPhaseSpaceGenerator();
  ~RTPhaseSpaceGenerator(){};
  void GeneratePrimaries(G4Event* evt);
 
private:
  int Init();
  void SetParticleGun();

private:
  G4String theFileName;
  iaea_header_type *p_iaea_header;
  iaea_record_type *p_iaea_record;
  G4double theNEvents;

  G4ParticleGun* theParticleGun;
};

#endif
