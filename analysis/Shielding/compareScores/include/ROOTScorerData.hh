#ifndef ROOTScorerData__HH
#define ROOTScorerData__HH
#include "ScorerData.hh"
class TH1F;

class ROOTScorerData : public ScorerData
{
public:
  ROOTScorerData(TH1F* histo);
  ~ROOTScorerData(){};

public:

  ROOTScorerData operator+=( const ROOTScorerData& sd2 );
  bool operator!=( const ROOTScorerData& sd2 );
  virtual void Print( std::ofstream& fout );

  virtual G4double GetIndexVal( G4String doseName, G4bool bLogX );
  
};

#endif
