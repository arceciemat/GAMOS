#ifndef GmGenerDistEnergyIsolethargical_h
#define GmGenerDistEnergyIsolethargical_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyIsolethargical : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyIsolethargical();
  virtual ~GmGenerDistEnergyIsolethargical(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theEnergyMin;
  G4double theEnergyMax;
  G4double theEnergyRatio;

};

#endif
