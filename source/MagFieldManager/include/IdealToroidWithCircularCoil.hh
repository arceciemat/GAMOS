///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef IdealToroidWithCircularCoil_HH
#define IdealToroidWithCircularCoil_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class IdealToroidWithCircularCoil: public G4MagneticField
{
  public:  // with description

    IdealToroidWithCircularCoil( G4double RToroid, G4double RoutCoil, G4double dCoil, G4double I);
    ~IdealToroidWithCircularCoil() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
    
    G4double r_toroid,dcoil,current, rout_coil;
    
   
};

#endif
