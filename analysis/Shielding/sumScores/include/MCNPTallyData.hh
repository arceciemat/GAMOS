#ifndef MCNPTallyData_hh
#define MCNPTallyData_hh

#include "ScoreData.hh"
#include "ScorerData.hh"
#include <fstream>

class MCNPTallyData : public ScoreData
{
public:
  MCNPTallyData( const std::vector<G4String>& wl );
  MCNPTallyData( G4String xVal, G4double tallyVal, G4double tallyErr );
  //  MCNPTallyData( const MCNPTallyData& file );
  ~MCNPTallyData(){};

  virtual void Print( std::ofstream& fout );

private:
  void SetVariables();

};

#endif
