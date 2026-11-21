#ifndef CSVScorerData__HH
#define CSVScorerData__HH
//---------------------------------------------------------------------------
// A 1D histogram ("1D" or "Prof1D") read from a GAMOS .csv histogram file:
// each bin (without underflow and overflow) is a CSVScoreData
//---------------------------------------------------------------------------
#include "ScorerData.hh"
#include <vector>

class CSVScorerData : public ScorerData
{
public:
  // contents and errors include underflow (index 0) and overflow (index nBins+1)
  CSVScorerData( const G4String& name, G4int nBins, G4double xMin, G4double xMax,
		 const std::vector<G4double>& contents, const std::vector<G4double>& errors );
  ~CSVScorerData(){};

  virtual void Print( std::ofstream& fout );

  // the score name is the bin centre
  virtual G4double GetIndexVal( G4String doseName, G4bool bLogX );
};

#endif
