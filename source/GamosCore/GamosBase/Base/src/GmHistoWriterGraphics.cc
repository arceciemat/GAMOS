#ifndef GAMOS_NO_ROOT
#include "TH1F.h"

#include "TCanvas.h"

#include "GmHistoWriterGraphics.hh"

//----------------------------------------------------------------------
GmHistoWriterGraphics::GmHistoWriterGraphics(const G4String& filename, const G4String& format) : theFileName(filename), theFormat(format)
{
  theCanvas = 0;

}

//----------------------------------------------------------------------
GmHistoWriterGraphics::~GmHistoWriterGraphics()
{
}

//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto1( const TH1F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto2( const TH2F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHistoProf1( const TProfile* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHistoProf2( const TProfile2D* his )
{
  SaveHisto( his );
}

//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto(  const TH1* his )
{
  if( !theCanvas ) {
    theCanvas = new TCanvas("gamosGif");
  }
  TH1* hisNC = const_cast<TH1*>(his);
  hisNC->Draw("");

  G4String histoName = theFileName + "." + his->GetName() + "." + theFormat;
  G4cout << "GmHistorWriterPDF::SaveHisto in file " << histoName << G4endl;
  theCanvas->Update();
  theCanvas->SaveAs(histoName.c_str(),"+");

}

#endif
