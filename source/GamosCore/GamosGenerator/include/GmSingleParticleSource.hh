#ifndef GmSingleParticleSource_H
#define GmSingleParticleSource_H
//
#include "GmParticleSource.hh"
#include "G4ParticleDefinition.hh"

class GmSingleParticleSource : public GmParticleSource
{
public:
  GmSingleParticleSource( const G4String& name, const G4String& partName, const G4double ener );
  ~GmSingleParticleSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );

  virtual G4String GetParticleName() const {
    return theParticleDef->GetParticleName(); }

private:

  G4ParticleDefinition* theParticleDef;

  G4bool bPolarization;
  G4double thePolarizationX;
  G4double thePolarizationY;
  G4double thePolarizationZ;
};

#endif // GmSingleParticleSource_H
