#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPad.h"
#include "TCanvas.h"
//#include <iostream>
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"
#include <fstream>

bool bHistoTXTErrors = true;
void SaveHisto1D(const std::string& hisType, const TH1* his );
void SaveHisto2D(const std::string& hisType, const TH2* his );
std::ofstream theFile;

//------------------------------------------------------------------------
void ROOT2TXT(const char* fileRoot, const char* fileTxt )
{
  TFile File( fileRoot );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  TH2F *histo2;
  
  int nh = 0;
  theFile.open(fileTxt);

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ) {
      histo1 = (TH1F*) File.Get( HistoKey->GetName() );
      if( hisname.find(".prof.") == std::string::npos ) {
	SaveHisto1D("1D",histo1);
      }
        SaveHisto1D("Prof1D",histo1);
      }  
    } else if( histo->GetDimension() == 2 ) {
      histo2 = (TH2F*) File.Get( HistoKey->GetName() );
      if( hisname.find(".prof.") == std::string::npos ) {
	SaveHisto1D("2D",histo2);
      } else {
	SaveHisto1D("Prof2D",histo2);
      }
    }

  }
  
}


//----------------------------------------------------------------------
void SaveHisto1D(const std::string& hisType, const TH1* his )
{
  int nbins = his->GetXaxis()->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbins << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax();
  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean();
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError();  
  }
  
  theFile << " RMS= " << his->GetRMS();  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError();  
  }    

  theFile << " UFLOW= " << his->GetBinContent(0);  
  theFile << " OFLOW= " << his->GetBinContent(nbins+1);  
  theFile << std::endl;  

  for( int ii = 1; ii <= nbins; ii++ ){
    theFile << his->GetXaxis()->GetXmin()+his->GetXaxis()->GetBinWidth(ii)*(ii-0.5) << " " << his->GetBinContent(ii);
    if( bHistoTXTErrors ) {
      theFile << " " << his->GetBinError(ii);
    }
    theFile << std::endl;
  }

}

//----------------------------------------------------------------------
void SaveHisto2D(const std::string& hisType, const TH2* his )
{
  int nbinsx = his->GetXaxis()->GetNbins();
  int nbinsy = his->GetYaxis()->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbinsx << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << " " << nbinsy << " " << his->GetYaxis()->GetXmin() << " " << his->GetYaxis()->GetXmax();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean(1);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(1);
  }
  
  theFile << " RMSX= " << his->GetRMS(1);  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError(1);  
  } 

  theFile << " MEANY= " << his->GetMean(2);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(2);
  }
  
  theFile << " RMSY= " << his->GetRMS(2);  
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(2) << std::endl;  
  }

  double flow;
  int ii, jj;
  flow = 0;
  ii = 0;
  for( jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWX= " << flow;

  flow = 0.;
  ii = nbinsx+1;
  for( jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWX= " << flow;

  flow = 0.;
  jj = 0;
  for( ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWY= " << flow;

  flow = 0.;
  jj = nbinsy+1;
  for( ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWY= " << flow;
  theFile << std::endl;

  for( ii = 0; ii <= nbinsx+1; ii++ ){
    for( jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << his->GetXaxis()->GetXmin()+his->GetXaxis()->GetBinWidth(ii)*(ii-0.5) 
	      << " " << his->GetYaxis()->GetXmin()+his->GetYaxis()->GetBinWidth(jj)*(jj-0.5) 
	      << " " << his->GetBinContent(ii,jj);
      if( bHistoTXTErrors ) {
	theFile << " " << his->GetBinError(ii,jj);
      } 
      theFile << std::endl;
    }
  }
 
}
