///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef ReplicateField_HH
#define ReplicateField_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class ReplicateField: public G4MagneticField
{
  public:  // with description

    ReplicateField(G4double Phi0, G4double dPhi, size_t Nreplicate,G4double R, G4MagneticField* aField);
    ~ReplicateField() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
   G4double phi0; 
   G4double dphi;
   size_t n_replicate;
   G4double r; 
   G4MagneticField* FieldToReplicate;
    
};

#endif
