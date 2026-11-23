///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MagneticDipole_HH
#define MagneticDipole_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"

class MagneticDipole: public G4MagneticField
{
  public:  // with description

    MagneticDipole(const G4ThreeVector& aDipoleMoment, const G4ThreeVector& aPos);
    ~MagneticDipole() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

    inline void SetDipoleMoment(const G4ThreeVector& aDipoleMoment) {theDipoleMoment= aDipoleMoment;}
    G4ThreeVector GetDipoleMoment(){return theDipoleMoment;}
    inline void SetDipoleCenter(const G4ThreeVector& aPos) {theDipoleCenter= aPos;}
    G4ThreeVector GetDipoleCenter(){return theDipoleCenter;}
    
    
    

  private:

    G4ThreeVector theDipoleMoment;
    G4ThreeVector theDipoleCenter;
};

#endif
