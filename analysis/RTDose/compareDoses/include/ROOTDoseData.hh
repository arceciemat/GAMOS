#ifndef ROOTDoseData__HH
#define ROOTDoseData__HH
#include "DoseData.hh"

class ROOTDoseData : public DoseData
{
public:
  ROOTDoseData( G4double binCenter, G4double binContents, G4double binError );
  ~ROOTDoseData(){};

  virtual void Print( std::ofstream& fout );

private:
};

#endif
