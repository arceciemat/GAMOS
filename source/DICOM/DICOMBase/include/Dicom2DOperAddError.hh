#ifndef Dicom2DOperAddError__HH
#define Dicom2DOperAddError__HH

#include "DicomVOperator.hh"
class DicomVImage;

class Dicom2DOperAddError 
{ 
public:
  Dicom2DOperAddError( G4int iOrder = INT_MAX, G4String name = "Dicom2DOperAddError", G4double nsig = 1);
  ~Dicom2DOperAddError(){};

public:
  virtual void Operate( DicomVImage*, DicomVImage* );
 
private:
  G4String theName;
  G4int theOrder;
  G4double theNSigmas;
  
};

#endif
  
