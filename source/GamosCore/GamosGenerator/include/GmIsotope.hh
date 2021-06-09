#ifndef GmIsotope_H
#define GmIsotope_H
//
#include "GmIsotopeDecay.hh"
#include <vector>

class GmIsotope
{
public:
  GmIsotope( const G4String& name );
  ~GmIsotope(){}

  void AddDecay( GmIsotopeDecay* dec );
  G4String GetName() const { return theName; }
  
  std::vector<GmIsotopeDecay*> GenerateDecayProducts();
    
private:
  G4String theName;
  std::vector<GmIsotopeDecay*> theDecays;

};

#endif // GmIsotope_H
