#ifndef GAMOSScoreData__HH
#define GAMOSScoreData__HH
#include "ScoreData.hh"

class GAMOSScoreData : public ScoreData
{
public:
  GAMOSScoreData(std::vector<G4String> wl, G4bool bTotal);
  ~GAMOSScoreData(){};

  virtual void Print( std::ofstream& fout );

private:
};

#endif
