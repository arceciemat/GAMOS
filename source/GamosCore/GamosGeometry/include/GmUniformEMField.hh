#ifndef GmUniformEMField_h
#define GmUniformEMField_h 1

#include "globals.hh"
#include "G4ElectroMagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

class GmUniformEMField : public G4ElectroMagneticField
{
  
public:
  GmUniformEMField(G4ThreeVector MagFieldVector, G4ThreeVector ElecFieldVector);
  virtual ~GmUniformEMField(){};

  virtual void GetFieldValue( const double Point[4], double *Bfield ) const;

  virtual G4bool DoesFieldChangeEnergy() const {
    return true;
  }

private: 
  double* theField;		       
};

#endif
