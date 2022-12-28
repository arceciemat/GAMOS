#include "globals.hh"
#include <vector>
#include<map>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"


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


//-----------------------------------------------------------------------
int main(int argc,char** argv) 
{
  int verbose = 1;
  //  new GmParameterMessenger;
  if(argc < 2 || argc > 4 ) {
    G4Exception("analyseOutput",
		"Error in argument",
		FatalErrorInArgument,
		"You must TWO-FOUR arguments, the filename logy type2d");
  }
  const char* filename = argv[1];
  bool logy = 0;
  std::string type2d = "colz"; 
  if( argc > 2 ) {
    logy = GmGenUtils::GetInt(argv[2]);
  }
  if( argc > 3 ) {
    type2d = argv[3];
  }
  
  TCanvas * theCanvas = new TCanvas("dd");
  theCanvas->cd();
  gStyle->SetOptStat(1111111);
  //  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  if( logy ) theCanvas->SetLogy(1);

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1 *histo1 = 0;
  TH2F *histo2 = 0;
  
  int nh = 0;
  std::string fn;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    TObject *obj = HistoKey->ReadObj();
    if( verbose >= 1 ) std::cout << " ROOT HISTO " << hisname << " " << dynamic_cast<TH1F*>(obj) << std::endl; //GDEB
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ) {
     //    if( dynamic_cast<TH1F*>(obj) ) {
      histo1 = dynamic_cast<TH1F*>(obj);
      histo1 = (TH1F*) File.Get( HistoKey->GetName() );
      if( verbose >= 1 ) std::cout << histo1 << " ROOT HISTO1 " << hisname << std::endl; //GDEB
      histo1->Draw("hE");      
      fn = std::string("his")+histo1->GetName() + std::string(".jpg");
      if( verbose >= 2 ) std::cout << " MEAN= " << histo1->GetMean() << " \"" << histo1->GetName() << "\"" << std::endl;
      if( verbose >= 2 ) std::cout << " RMS= " << histo1->GetRMS() << " \"" << histo1->GetName() << "\"" << std::endl;
      if( verbose >= 2 ) std::cout << " NENTRIES= " << histo1->GetEntries() << " \"" << histo1->GetName() << "\"" << std::endl;
      if( verbose >= 2 ) std::cout << "HISTO_LIMITS= " << histo1->GetXaxis()->GetXmin() << " " << histo1->GetXaxis()->GetXmax() << " \"" << histo1->GetName() << "\" " << std::endl; 
      /*      for( int ii = 0; ii < histo1->GetXaxis()->GetNbins(); ii++ ) {
	if( histo1->GetBinContent(ii) != 0 ) {
	  std::cout << ii << " DATA= " << " " << histo1->GetBinContent(ii) << std::endl; //GDEB
      //      std::cout << histo1->GetXaxis()->BinCenter(ii) << " DATA= " << histo1->GetBinContent(ii) << std::endl;
	}
	}*/

    } else if( histo->GetDimension() == 2 ) {
      //    } else if( dynamic_cast<TH2F*>(obj) ) {
      histo2 = dynamic_cast<TH2F*>(obj);
      histo2 = (TH2F*) File.Get( HistoKey->GetName() );
      histo2->Draw(type2d.c_str());
      fn = std::string("his")+histo2->GetName() + std::string(".jpg");
      if( verbose >= 2 ) std::cout << " MEAN= " << histo2->GetMean() << " \"" << histo2->GetName() << "\" " << std::endl;
      if( verbose >= 2 ) std::cout << " RMS= " << histo2->GetRMS() << " \"" << histo2->GetName() << "\" " << std::endl;
      if( verbose >= 2 ) std::cout << " NENTRIES= " << histo2->GetEntries() << " \"" << histo2->GetName() << "\" " << std::endl;
      if( verbose >= 2 ) std::cout << "HISTO_LIMITS= " << histo2->GetXaxis()->GetXmin() << " " << histo2->GetXaxis()->GetXmax() << " " << histo2->GetYaxis()->GetXmin() << " " << histo2->GetYaxis()->GetXmax() << " \"" << histo2->GetName() << "\" " << std::endl;
      if( verbose >= 3 ) { 
	for( int ix = 0; ix < histo2->GetXaxis()->GetNbins()+2; ix++ ) {
	  for( int iy = 0; iy < histo2->GetYaxis()->GetNbins()+2; iy++ ) {
	    if( histo2->GetBinContent(ix,iy) != 0 ) {
	      std::cout << ix << " : " << iy << " DATA= " << histo2->GetBinContent(ix,iy) << "+-" << histo2->GetBinError(ix,iy) << std::endl; //GDEB
	      //	    std::cout << histo2->GetXaxis()->BinCenter(ix) << " : " << histo2->GetYaxis()->BinCenter(iy) << " DATA= " << histo2->GetBinContent(ix,iy) << std::endl; 
	    }
	  }
	}
      }
    }

    theCanvas->Update();

    // change ':'
    std::replace( fn.begin(), fn.end(), '/', '_'); 
    theCanvas->Print(fn.c_str());
    //    std::cout << " fn " << fn << std::endl; //GDEB
    
  }
  
}
