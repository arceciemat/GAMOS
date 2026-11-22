#ifndef TextScoreData__HH
#define TextScoreData__HH
#include "ScoreData.hh"

class TextScoreData : public ScoreData
{
public:
  TextScoreData(std::vector<G4String> wl );
  ~TextScoreData(){};

  virtual void Print( std::ofstream& fout );

private:
};

#endif
