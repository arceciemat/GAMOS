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
void divideAll(const char* fileOutName, const char* fileInName, int nDiv )
{
  TFile* FileOut = new TFile(fileOutName, "RECREATE");
  if( !FileOut->IsOpen() ){
    std::cerr << " ERROR CREATING FILE " << fileOutName << std::endl;
    exit(1);
  }
    
  TFile File( fileInName );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  TH2F *histo2;
  
  int nh = 0;
  std::string fn;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) HistoKey->ReadObjectAny(TH1::Class()); // File.Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ) {
      histo1 = (TH1F*) histo;
      TAxis* xAxis = histo1->GetXaxis();
      for( int ii = 0; ii < xAxis->GetNbins()+1; ii++ ) {
	histo1->SetBinContent(ii,histo1->GetBinContent(ii)/nDiv);
	histo1->SetBinError(ii,histo1->GetBinError(ii)/nDiv);       
      }
	//      (*histo1)*(1./nDiv);
      FileOut->cd();
      histo1->Write();
    } else if( histo->GetDimension() == 2 ) {
      histo2 = (TH2F*) histo;
      TAxis* xAxis = histo2->GetXaxis();
      TAxis* yAxis = histo2->GetYaxis();
      for( int ix = 0; ix < xAxis->GetNbins()+1; ix++ ) {
	for( int iy = 0; iy < xAxis->GetNbins()+1; iy++ ) {
	  histo2->SetBinContent(ix,iy,histo2->GetBinContent(ix,iy)/nDiv);
	  histo2->SetBinError(ix,iy,histo2->GetBinError(ix,iy)/nDiv);       
	}
      }
      FileOut->cd();
      histo2->Write();
    }
    
  }

  FileOut->Close();

}
