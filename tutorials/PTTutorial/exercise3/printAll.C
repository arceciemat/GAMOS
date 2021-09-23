#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPad.h"
#include "TCanvas.h"
#include <iostream>
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"

//------------------------------------------------------------------------
void printAll(const char* filename, bool logy = 0, char* type2d = 0) 
{
  TCanvas * theCanvas = new TCanvas("dd");
  theCanvas->cd();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  if( logy ) theCanvas->SetLogy(1);

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  TH2F *histo2;
  
  int nh = 0;
  std::string fn;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ) {
      histo1 = (TH1F*) File.Get( HistoKey->GetName() );
      histo1->Draw("hE");
      fn = std::string("his")+histo->GetName() + std::string(".gif");
      std::cout << " MEAN= " << histo->GetMean() << histo->GetName() << std::endl;
      std::cout << " RMS= " << histo->GetRMS() << histo->GetName() << std::endl;
      std::cout << " NENTRIES= " << histo->GetEntries() << histo->GetName() << std::endl;
    } else if( histo->GetDimension() == 2 ) {
      histo2 = (TH2F*) File.Get( HistoKey->GetName() );
      //      histo2->Draw(type2d);
      histo2->Draw("logz");
      fn = std::string("his")+histo2->GetName() + std::string(".gif");
      std::cout << " MEAN= " << histo->GetMean() << histo->GetName() << std::endl;
      std::cout << " RMS= " << histo->GetRMS() << histo->GetName() << std::endl;
      std::cout << " NENTRIES= " << histo->GetEntries() << histo->GetName() << std::endl;
    }

    theCanvas->Update();

    theCanvas->Print(fn.c_str());
    
  }
  
}
