#ifndef GmHistoWriterGraphics_h
#define GmHistoWriterGraphics_h 1

#ifndef GAMOS_NO_ROOT
#include "GmHistoWriter.hh"
class TCanvas;
//class GmHisto1;

#include "globals.hh"

class GmHistoWriterGraphics : public GmHistoWriter
{
public:

  GmHistoWriterGraphics(const G4String& filename, const G4String& format);
  ~GmHistoWriterGraphics();
  virtual void SaveHisto1( const TH1F* his );
  virtual void SaveHisto2( const TH2F* his );
  virtual void SaveHistoProf1( const TProfile* his );
  virtual void SaveHistoProf2( const TProfile2D* his );
  //  virtual void SaveHisto1( const GmHisto1* gmhis );

 private:
  void SaveHisto(  const TH1* his );
  //  void SaveHisto( const GmHisto1* gmhis );

  private:
  G4String theFileName;
  G4String theFormat;
  TCanvas* theCanvas;
};

#endif

#endif
