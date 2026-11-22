#ifndef DicomOperLog10__HH
#define DicomOperLog10__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperLog10 : public DicomVOperator
{ 
public:
  DicomOperLog10( G4int iOrder = INT_MAX, G4String name = "DicomOperLog10");
  ~DicomOperLog10(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
};

#endif
  
