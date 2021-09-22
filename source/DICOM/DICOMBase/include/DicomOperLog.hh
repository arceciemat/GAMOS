#ifndef DicomOperLog__HH
#define DicomOperLog__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperLog : public DicomVOperator
{ 
public:
  DicomOperLog( G4int iOrder = INT_MAX, G4String name = "DicomOperLog");
  ~DicomOperLog(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
};

#endif
  
