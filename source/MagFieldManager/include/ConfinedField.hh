///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef ConfinedField_HH
#define ConfinedField_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class ConfinedField: public G4MagneticField
{
  public:  // with description

    ConfinedField(      G4MagneticField* aField, G4double var1_min,G4double var1_max, 
    			G4double var2_min,G4double var2_max, 
			G4double var3_min,G4double var3_max,
			G4bool aBool);
    ~ConfinedField() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
    G4MagneticField* theField;
    G4double phi_min,phi_max,rho_min,rho_max;
    G4double x_min,x_max,y_min,y_max,z_min,z_max;
    G4bool IsCartesian; 
    	
};

#endif
