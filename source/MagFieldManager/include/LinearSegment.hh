///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef LinearSegment_HH
#define LinearSegment_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
class BiotSavartIntegrator;

class LinearSegment: public G4MagneticField
{
  public:  // with description

    LinearSegment(G4double D1, G4double D2,G4double L,G4double current);
    ~LinearSegment();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;
    
    
    
    

  private:
    G4double d1,d2, I;
    G4double l;
    BiotSavartIntegrator* theBiotSavartIntegrator;
    
    
    

    
   
  
  
};

#endif

