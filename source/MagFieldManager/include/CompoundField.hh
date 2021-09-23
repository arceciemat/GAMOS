///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CompoundField_HH
#define CompoundField_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4AffineTransform.hh"

#include <vector>
class CompoundField: public G4MagneticField
{
  public:  // with description

    CompoundField(const std::vector<G4MagneticField*> aList_of_fields,const std::vector<G4AffineTransform*> aList_of_transformations );
    ~CompoundField() ;

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;

   
    

  private:
    std::vector<G4MagneticField*> list_of_fields;
    std::vector<G4AffineTransform*> list_of_transformations;
    std::vector<G4AffineTransform*> list_of_inverse_transformations;	
};

#endif
