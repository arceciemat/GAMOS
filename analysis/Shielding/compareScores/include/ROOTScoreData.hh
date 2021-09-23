#ifndef ROOTScoreData__HH
#define ROOTScoreData__HH
#include "ScoreData.hh"

class ROOTScoreData : public ScoreData
{
public:
  ROOTScoreData( G4double binCenter, G4double binContents, G4double binError );
  ~ROOTScoreData(){};

  virtual void Print( std::ofstream& fout );

private:
};

#endif
