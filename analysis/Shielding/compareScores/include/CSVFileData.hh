#ifndef CSVFileData__HH
#define CSVFileData__HH
//---------------------------------------------------------------------------
// GAMOS .csv histogram file (written by GmHistoWriterCSV, can be read with
// histBrowser.py), read with GmHistoReaderCSV. Each 1D histogram ("1D" or
// "Prof1D") is a scorer with the histogram name; 2D histograms are not used.
// GmHistoReaderCSV checks the file: a line without errors or with a field that
// is nan, inf or not a number is a fatal error.
// It can be used with and without ROOT.
//---------------------------------------------------------------------------
#include <vector>
#include <fstream>
#include "globals.hh"

#include "FileData.hh"

class CSVFileData : public FileData
{
public:
  CSVFileData(G4String fileName, G4bool bInvertX);
  ~CSVFileData(){};

  virtual void Print( std::ofstream& fout );

private:
  // add one scorer per histogram of the map (GmHisto1 or GmHistoProfile1, that are ROOT TH1F or TProfile with ROOT)
  template<class H> void AddHistos( const std::map<G4String,H*>& histos );
};

#endif
