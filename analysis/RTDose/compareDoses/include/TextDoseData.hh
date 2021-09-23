#ifndef TextDoseData__HH
#define TextDoseData__HH
#include "DoseData.hh"

class TextDoseData : public DoseData
{
public:
  TextDoseData(std::vector<G4String> wl );
  ~TextDoseData(){};

  virtual void Print( std::ofstream& fout );

private:
};

#endif
