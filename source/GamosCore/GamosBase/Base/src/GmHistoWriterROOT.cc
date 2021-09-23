#ifndef GAMOS_NO_ROOT
#include "GmAnalysisMgr.hh"

#include "TH1F.h"

#include "TFile.h"

#include "GmHistoWriterROOT.hh"

//----------------------------------------------------------------------
GmHistoWriterROOT::GmHistoWriterROOT(const G4String& filename)
{
  //  theFile = new TFile(filename.c_str(),"recreate");
  //  G4cout << "GmHistoWriterROOT: creating histo file " << filename << G4endl;
  theFileName = filename;  
  theFile = 0;

}

//----------------------------------------------------------------------
GmHistoWriterROOT::~GmHistoWriterROOT()
{
  theFile->Close();
  //t  delete theFile;
}

/*
//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHisto1( const GmHisto1* his )
{
  SaveHisto( his );
}
*/

//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHisto1( const TH1F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHisto2( const TH2F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHistoProf1( const TProfile* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHistoProf2( const TProfile2D* his )
{
  SaveHisto( his );
}

//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHisto(  const TH1* his )
{
  if( !theFile ) {
    theFile = new TFile(theFileName.c_str(),"recreate");
  }
  his->Write();

  /*  G4int nbins = his->GetXaxis()->GetNbins();
  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    if( his->GetBinContent(ii) != 0 )  G4cout << "ROOT set bin content " << ii << " = " << his->GetBinContent(ii) << " +- " << his->GetBinError(ii) << G4endl;
    } */

  //  G4cout << " ROOT mean " << his->GetMean() << " RMS " << his->GetRMS() << " NENT " << his->GetEntries() << G4endl;

  //  G4cout << " ROOT fTsumwx " << his->fTsumwx
  //	 << " fTsumwx2 " << his->fTsumwx2 
  //	 << " fTsumw " << his->fTsumw
  //	 << " fTsumw2 " << his->fTsumw2 << G4endl;
}


/*
//----------------------------------------------------------------------
void GmHistoWriterROOT::SaveHisto( const GmHisto1* gmhis )
{
  TH1F* rhis = new TH1F( gmhis->GetName().c_str(), gmhis->GetName().c_str(), gmhis->GetNBins(), gmhis->GetLowerEdge(), gmhis->GetUpperEdge() );
  //rhis->fTsumW

  //----- Copy bins
  for( int ii = 0; ii <= gmhis->GetNBins()+1; ii++ ){
    double binc = gmhis->GetBinContent(ii);
    rhis->SetBinContent(ii, binc);
    rhis->SetBinError(ii, gmhis->GetBinError(ii));
    //    if( gmhis->GetBinContent(ii) != 0 ) G4cout << "Gm set bin content " << ii << " = " << binc << " +- " << gmhis->GetBinError(ii) << G4endl;
  }

  //----- To compute mean and rms
  rhis->fTsumwx = gmhis->GetSumWX();
  rhis->fTsumwx2 = gmhis->GetSumWX2();
  rhis->fTsumw = gmhis->GetSumW();
  rhis->fTsumw2 = gmhis->GetSumW2();
  rhis->fEntries = gmhis->GetEntries();

  //  G4cout << " Gm mean " << gmhis->GetMean() << " RMS " << gmhis->GetRMS() << " NENT " << gmhis->GetEntries() << G4endl;
  // G4cout << " Gm->ROOT mean " << rhis->GetMean() << " RMS " << rhis->GetRMS() << " NENT " << rhis->GetEntries() << G4endl;

  //  G4cout << " Gm fTsumwx " <<  gmhis->GetSumWX()
  //	 << " fTsumwx2 " <<  gmhis->GetSumWX2()
  //	 << " fTsumw " << gmhis->GetSumW()
  //	 << " fTsumw2 " <<  gmhis->GetSumW2() << G4endl;

  if( !theFile ) theFile = new TFile(theFileName.c_str(),"recreate");

  rhis->Write();
}
*/

#endif
