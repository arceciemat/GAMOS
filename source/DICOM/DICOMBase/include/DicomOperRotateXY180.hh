#ifndef DicomOperRotateXY180__HH
#define DicomOperRotateXY180__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperRotateXY180 : public DicomVOperator
{ 
public:
  DicomOperRotateXY180( G4double angle, G4int iOrder = INT_MAX, G4String name = "DicomOperRotateXY180");
  ~DicomOperRotateXY180(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
  G4double theAngle;
};

#endif
  
