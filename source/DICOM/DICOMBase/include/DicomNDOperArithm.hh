#ifndef DicomNDOperArithm__HH
#define DicomNDOperArithm__HH

#include "DicomVOperator.hh"
#include <vector>
class DicomVImage;

class DicomNDOperArithm 
{ 
public:
  DicomNDOperArithm( G4int iOrder, G4String name, G4String arithm);
  ~DicomNDOperArithm(){};

public:
  virtual void Operate( DicomVImage* newImage, std::vector<DicomVImage*> images );
 
private:
  G4String theName;
  G4int theOrder;
  G4String theArithmeticOperation;
  G4int theNData;
  G4bool bChecked;
};

#endif
  
