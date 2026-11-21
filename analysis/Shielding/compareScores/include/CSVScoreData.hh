#ifndef CSVScoreData__HH
#define CSVScoreData__HH
//---------------------------------------------------------------------------
// One bin of a histogram read from a GAMOS .csv histogram file
// (GmHistoWriterCSV, the files that can be read with histBrowser.py).
// As ROOTScoreData, the score name is the bin centre; the error is the bin error
// written in the file (0 if the file was written without errors).
//---------------------------------------------------------------------------
#include "ScoreData.hh"

class CSVScoreData : public ScoreData
{
public:
  CSVScoreData( G4double binCenter, G4double binContents, G4double binError );
  ~CSVScoreData(){};

  virtual void Print( std::ofstream& fout );
};

#endif
