#ifndef DicomOperPrintValueAtPoint__HH
#define DicomOperPrintValueAtPoint__HH

#include "DicomVOperator.hh"
#include "G4ThreeVector.hh"
class DicomVImage;

class DicomOperPrintValueAtPoint : public DicomVOperator
{ 
public:
  DicomOperPrintValueAtPoint( G4ThreeVector pt, G4int iOrder = INT_MAX, G4String name = "DicomOperPrintValueAtPoint");
  ~DicomOperPrintValueAtPoint(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
  G4ThreeVector thePoint;
};

#endif
  
