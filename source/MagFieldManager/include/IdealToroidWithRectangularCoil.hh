///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef IdealToroidWithRectangularCoil_HH
#define IdealToroidWithRectangularCoil_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class IdealToroidWithRectangularCoil: public G4MagneticField
{
  public:  // with description

    IdealToroidWithRectangularCoil( G4double RToroid,G4double Lhoriz,G4double Lvert,G4double RoutCorner,G4double dCoil, G4double I);
    ~IdealToroidWithRectangularCoil() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
    
    G4double rout_corner,dcoil,current;
    G4double z_max1,z_max2;
    G4double rho_min1,rho_min2;
    G4double rho_max1,rho_max2;
};

#endif
