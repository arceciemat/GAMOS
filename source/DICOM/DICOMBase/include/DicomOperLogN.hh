#ifndef DicomOperLogN__HH
#define DicomOperLogN__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperLogN : public DicomVOperator
{ 
public:
  DicomOperLogN( G4double logn, G4int iOrder = INT_MAX, G4String name = "DicomOperLogN");
  ~DicomOperLogN(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
  G4double theLogNumber;
};

#endif
  
