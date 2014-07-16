#ifndef GmHistoReaderCSV_h
#define GmHistoReaderCSV_h 1

#include <fstream>

#include "globals.hh"
#ifdef GAMOS_NO_ROOT
#include "GmHisto1.hh"
#include "GmHisto2.hh"
#include "GmHistoProfile1.hh"
#include "GmHistoProfile2.hh"
#else
#include "GmAnalysisMgr.hh" // typedef's to ROOT histos
#endif

#include "GamosCore/GamosUtils/include/GmFileIn.hh"

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

  GmHisto1* GetHisto1( const G4String& histoName );
  GmHisto2* GetHisto2( const G4String& histoName );
  GmHistoProfile1* GetHistoProfile1( const G4String& histoName );
  GmHistoProfile2* GetHistoProfile2( const G4String& histoName );

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

private:
  G4String theFileName;
  GmFileIn theFile;

  std::map<G4String,GmHisto1*> theHistos1;
  std::map<G4String,GmHisto2*> theHistos2;
  std::map<G4String,GmHistoProfile1*> theHistosProfile1;
  std::map<G4String,GmHistoProfile2*> theHistosProfile2;

  static G4bool bHistoCSVErrors;

};

#endif
