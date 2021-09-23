#include"MultiplyField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"

////////////////////////////////////////////////////////////////////////////////
//
MultiplyField::MultiplyField(G4MagneticField* aField, G4double f)
{ theFieldToMultiply=aField;
  factor = f;
} 
////////////////////////////////////////////////////////////////////////////////
//
MultiplyField::~MultiplyField()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void MultiplyField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{
  theFieldToMultiply->GetFieldValue(yTrack,MagField);
  MagField[0]= MagField[0]*factor;
  MagField[1]= MagField[1]*factor;
  MagField[2]= MagField[2]*factor;
}
