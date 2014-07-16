#ifndef GmIsotopeDecay_HH
#define GmIsotopeDecay_HH
//
#include "globals.hh"
class G4ParticleDefinition;

class GmIsotopeDecay 
{
public:
  GmIsotopeDecay( const G4String& energy, const G4String& prob, const G4String& product );
  ~GmIsotopeDecay(){}

  G4double GetEnergy() const { return theEnergy; }
  G4double GetProbability() const { return theProbability; }
  G4ParticleDefinition* GetProduct() const { return theProduct; }

  G4String GetProductName() const;


private:  
  G4double theEnergy;
  G4double theProbability;
  G4ParticleDefinition* theProduct;
  
};

#endif // GmIsotopeDecay_H
