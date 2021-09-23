#ifndef ROOTDoserData__HH
#define ROOTDoserData__HH
#include "DoserData.hh"
class TH1F;

class ROOTDoserData : public DoserData
{
public:
  ROOTDoserData(TH1F* histo);
  ~ROOTDoserData(){};

public:

  ROOTDoserData operator+=( const ROOTDoserData& sd2 );
  bool operator!=( const ROOTDoserData& sd2 );
  virtual void Print( std::ofstream& fout );

  virtual G4double GetIndexVal( G4String doseName, G4bool bLogX );
  
};

#endif
