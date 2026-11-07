#ifndef DicomOperReflectY__HH
#define DicomOperReflectY__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperReflectY : public DicomVOperator
{ 
public:
  DicomOperReflectY( G4int iOrder = INT_MAX, G4String name = "DicomOperReflectY");
  ~DicomOperReflectY(){};

public:
  virtual void Operate( DicomVImage* );
 
};

#endif
  
