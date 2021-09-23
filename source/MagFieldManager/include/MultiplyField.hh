///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MultiplyField_HH
#define MultiplyField_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"

class MultiplyField: public G4MagneticField
{
  public:  // with description

    MultiplyField(G4MagneticField* aField, G4double f);
    ~MultiplyField() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

  
    
    
    

  private:
    G4MagneticField* theFieldToMultiply;
    G4double factor;
    
  
  
};

#endif
