#ifndef GmDoubleBackToBackParticleSource_H
#define GmDoubleBackToBackParticleSource_H
//
#include "GmParticleSource.hh"
#include "G4ParticleDefinition.hh"

class GmDoubleBackToBackParticleSource : public GmParticleSource
{
public:
  GmDoubleBackToBackParticleSource( const G4String& name, const G4String& partName, const G4double ener );
  ~GmDoubleBackToBackParticleSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );

  virtual G4String GetParticleName() const {
    return theParticleDef->GetParticleName(); }

private:

  G4ParticleDefinition* theParticleDef;
};

#endif // GmDoubleBackToBackParticleSource_H
