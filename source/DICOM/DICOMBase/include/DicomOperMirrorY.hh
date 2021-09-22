#ifndef DicomOperMirrorY__HH
#define DicomOperMirrorY__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperMirrorY : public DicomVOperator
{ 
public:
  DicomOperMirrorY( G4int iOrder = INT_MAX, G4String name = "DicomOperMirrorY");
  ~DicomOperMirrorY(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
};

#endif
  
