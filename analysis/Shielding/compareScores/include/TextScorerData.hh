#ifndef TextScorerData__HH
#define TextScorerData__HH
#include "ScorerData.hh"

class TextScorerData : public ScorerData
{
public:
  TextScorerData();
  ~TextScorerData(){};

public:

  TextScorerData operator+=( const TextScorerData& sd2 );
  bool operator!=( const TextScorerData& sd2 );
  virtual void Print( std::ofstream& fout );

  virtual G4double GetIndexVal( G4String doseName, G4bool bLogX );
  
};

#endif
