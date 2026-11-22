///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CircularCoil_HH
#define CircularCoil_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"

class CircularCoil: public G4MagneticField
{
  public:  // with description

    CircularCoil(G4double R,G4double dr, G4double dz,G4double current);
    ~CircularCoil();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

    inline void SetRadius(G4double aRad) {Radius= aRad;}
    inline void SetDR(G4double aVal) {DR= aVal;}
    inline void SetDZ(G4double aVal) {DZ= aVal;}
    inline void SetI(G4double aVal) {I= aVal;}
    
    
    

  private:
    G4double Radius;
    G4double DR,DZ;
    G4double I;
    
    void BCurrentloop(G4double a, G4double i, G4double rho, G4double z, G4double& Brho, G4double& Bz) const;
    void SumOfLoops(G4int nr, G4int nz,G4double rho, G4double z, G4double& Brho, G4double& Bz) const;
  
  
};

#endif
