///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CurvedSegment_HH
#define CurvedSegment_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
class BiotSavartIntegrator;

class CurvedSegment: public G4MagneticField
{
  public:  // with description

    CurvedSegment(G4double D1, G4double D2,G4double R, G4double phi0, G4double dPhi, G4double current);
    ~CurvedSegment();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;
    
    
    
    

  private:
    G4double d1,I;
    G4double rin,rout,phi0,dphi;
    BiotSavartIntegrator* theBiotSavartIntegrator;
    
    
    

    
   
  
  
};

#endif

