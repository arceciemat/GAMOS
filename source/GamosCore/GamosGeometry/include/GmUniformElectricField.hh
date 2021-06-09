#ifndef GmUniformElectricField_HH
#define GmUniformElectricField_HH

#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4ElectricField.hh"

class GmUniformElectricField 
{
public:  // with description
  
  GmUniformElectricField(const G4ThreeVector FieldVector );
  
  ~GmUniformElectricField(){ };

};

#endif
