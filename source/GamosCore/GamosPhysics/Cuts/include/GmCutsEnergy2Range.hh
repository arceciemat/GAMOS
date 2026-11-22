#ifndef GmCutsEnergy2Range__HH
#define GmCutsEnergy2Range__HH

class G4MaterialCutsCouple; 
class G4Material; 
class G4ParticleDefinition;
class G4RToEConvForGamma;
class G4RToEConvForElectron;
class G4RToEConvForPositron;
class G4VRangeToEnergyConverter;

#include "globals.hh"

class GmCutsEnergy2Range
{
public:
  GmCutsEnergy2Range();
  ~GmCutsEnergy2Range();

  G4double Convert( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* mate );
  G4double ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple );
  G4double ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4Material* mate );
  G4double Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple);
  G4double Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* part, const G4Material* mate);

private:
  G4RToEConvForGamma * theR2EGamma;
  G4RToEConvForElectron * theR2EElectron;
  G4RToEConvForPositron * theR2EPositron;
};

#endif
