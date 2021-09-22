#ifndef DicomOperSoften__HH
#define DicomOperSoften__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperSoften : public DicomVOperator
{ 
public:
  DicomOperSoften( G4int iOrder = INT_MAX, G4String name = "DicomOperSoften");
  ~DicomOperSoften(){};

public:
  virtual void Operate( DicomVImage* );
  void Operate( DicomVImage* image, G4double factor, G4int nAround, G4double maxValue, G4double minSpareValue, G4double maxSpareValue );
  
private:
  G4double theFactor;
  G4int theNAround;
  G4double theMaxValue;
  G4double theMinSpareValue;
  G4double theMaxSpareValue;
};

#endif
  
