#ifndef DicomOperPrintMax__HH
#define DicomOperPrintMax__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperPrintMax : public DicomVOperator
{ 
public:
  DicomOperPrintMax( G4int iOrder = INT_MAX, G4String name = "DicomOperPrintMax");
  ~DicomOperPrintMax(){};

public:
  virtual void Operate( DicomVImage* );
 
private:
  G4double theMax;
};

#endif
  
