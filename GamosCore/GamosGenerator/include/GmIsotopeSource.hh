#ifndef GmIsotopeSource_H
#define GmIsotopeSource_H
//
#include "GmParticleSource.hh"
#include "GmIsotope.hh"

class GmIsotopeSource : public GmParticleSource
{
public:
  GmIsotopeSource( const G4String& name,  GmIsotope* isotope, G4double activity );
  ~GmIsotopeSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );

  virtual G4String GetParticleName() const {
    return theIsotope->GetName(); }

  GmIsotopeDecay* GetCurrentDecay() const { return theCurrentDecay; }

private:
  GmIsotope* theIsotope;

  GmIsotopeDecay* theCurrentDecay; 

};

#endif // GmIsotopeSource_H
