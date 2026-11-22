#ifndef GmHistoReaderCSV_h
#define GmHistoReaderCSV_h 1

#include "globals.hh"
#include <map>
#include <vector>
#ifndef GAMOS_ROOT
class GmHisto1;
class GmHisto2;
class GmHistoProfile1;
class GmHistoProfile2;
#else // same typedef's as in GmAnalysisMgr.hh
class TH1F;
class TH2F;
class TProfile;
class TProfile2D;
typedef TH1F GmHisto1;
typedef TH2F GmHisto2;
typedef TProfile GmHistoProfile1;
typedef TProfile2D GmHistoProfile2;
#endif

#include "GamosCore/GamosUtils/include/GmFileIn.hh"

// Reads histograms of a GAMOS .csv file (written by GmHistoWriterCSV, always with bin errors).
// A line with a wrong number of fields (e.g. without errors), or with a field that is
// nan, inf or not a number, is a fatal error
class GmHistoReaderCSV
{
public:

  GmHistoReaderCSV(const G4String& filename);
  ~GmHistoReaderCSV();

  void ReadFile();

  void FillHisto1D( std::vector<G4String>& wl );
  void FillHisto2D( std::vector<G4String>& wl );
  void FillHistoProfile1D( std::vector<G4String>& wl );
  void FillHistoProfile2D( std::vector<G4String>& wl );

  GmHisto1* GetHisto1( const G4String& histoName, G4bool bExists = true );
  GmHisto2* GetHisto2( const G4String& histoName, G4bool bExists = true );
  GmHistoProfile1* GetHistoProfile1( const G4String& histoName, G4bool bExists = true );
  GmHistoProfile2* GetHistoProfile2( const G4String& histoName, G4bool bExists = true );

  std::map<G4String,GmHisto1*> GetHistos1() const {
    return theHistos1;
  }
  std::map<G4String,GmHisto2*> GetHistos2() const {
    return theHistos2;
  }
  std::map<G4String,GmHistoProfile1*> GetHistosProfile1() const {
    return theHistosProfile1;
  }
  std::map<G4String,GmHistoProfile2*> GetHistosProfile2() const {
    return theHistosProfile2;
  }

private:
  void FillHisto1D( std::vector<G4String>& wl, GmHisto1* his );
  void FillHisto2D( std::vector<G4String>& wl, GmHisto2* his );
  // field iw of the current line as a number: fatal error if it is nan, inf or not a number
  G4double GetNumber( const std::vector<G4String>& wl, size_t iw );
  // fatal error if the number of fields is not nExpected
  void CheckNFields( const std::vector<G4String>& wl, size_t nExpected );

private:
  G4String theFileName;
  GmFileIn theFile;

  std::map<G4String,GmHisto1*> theHistos1;
  std::map<G4String,GmHisto2*> theHistos2;
  std::map<G4String,GmHistoProfile1*> theHistosProfile1;
  std::map<G4String,GmHistoProfile2*> theHistosProfile2;

};

#endif
