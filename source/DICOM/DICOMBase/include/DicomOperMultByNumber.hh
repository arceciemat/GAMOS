#ifndef DicomOperMultByNumber__HH
#define DicomOperMultByNumber__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperMultByNumber : public DicomVOperator
{ 
public:
  DicomOperMultByNumber( G4double multn, G4int iOrder = INT_MAX, G4String name = "DicomOperMultByNumber");
  ~DicomOperMultByNumber(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
  G4double theMultNumber;
};

#endif
  
