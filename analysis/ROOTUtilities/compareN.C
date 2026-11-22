#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TH1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TLegend.h>
#include <TPostScript.h>
#include <TPaveLabel.h>
#include <TText.h>
#include <TMath.h>
#include <TF1.h>
#include <TFile.h>
#include <TLine.h>
#include <TPaveStats.h>

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#endif

bool theOptLogy;
std::vector<TFile*> theFiles;
TCanvas* theCanvas;

TPostScript* myps;

void setStyle();
TH1F* getHisto1(int theFileNumber, const char* hnam );
void compareNHis(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm );
void compareNHis( std::vector<TH1F*> histos, const char* XName, const char* YName, const char* OutFileName, bool bnorm );
void SetAttributes( TH1F* his, int iflag);

char * theGlobalTitle;
std::vector<char*> theSampleTitle;
int nFiles;

//------------------------------------------------------------------------
void InitComparison(std::vector<const char*>* fileNames )
{

  theOptLogy = 0;  
  setStyle();

  nFiles = int(fileNames->size());
  for( int ii = 0; ii < nFiles; ii++ ) {
    theFiles.push_back( new TFile((*fileNames)[ii] ) );
    std::cout << " FILES " << (*fileNames)[ii]  << " " << theFiles.size() << std::endl;
  }

  theCanvas = new TCanvas("dd");
  theCanvas->cd();

}


//------------------------------------------------------------------------
void compareNHis(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm )
{
  std::vector<TH1F*> histos;
  for( unsigned int ii = 0; ii < nFiles; ii++ ) {
    histos.push_back( getHisto1(ii, hnam) );
  }
  compareNHis( histos, XName, YName, OutFileName, bnorm );
}


//------------------------------------------------------------------------
void compareNHis( std::vector<TH1F*> histos, const char* XName, const char* YName, const char* OutFileName, bool bnorm )
{
  // First the attributes ....
  for( int ii = 0; ii < nFiles; ii++ ) {
  }

  histos[0]->SetXTitle(XName);
  histos[0]->SetYTitle(YName);

  TLegend* lege = new TLegend(0.75,0.75,0.98,1.0);
  lege->SetTextAlign(3);
  lege->SetTextSize(0.1);
  lege->SetTextColor(1);
  for( int ii = 0; ii < nFiles; ii++ ) {
    lege->AddEntry(histos[ii],theSampleTitle[ii],"P");
  }

  TPaveLabel* tit_eta_glo = new TPaveLabel(0.0,0.0,1.0,1.0,theGlobalTitle);
  tit_eta_glo->SetTextColor(1);

  theCanvas->Update();
  theCanvas->cd();

  gStyle->SetOptStat(111111);
  std::vector<TPad*> pads; 
  for( int ii = 0; ii < nFiles; ii++ ) {    
    TPad* pad = new TPad("pada","Pad a",float(ii)/nFiles,0.5,float(ii+1)/nFiles,1.);
    pads.push_back( pad );
    pad->SetLogy(theOptLogy);
    pad->Draw();
  } 
  TPad* pad2 = new TPad("pad2","Pad 2",0,0,1,0.5);
  pad2->Draw();

  gStyle->SetOptStat(111111);
  for( int ii = 0; ii < nFiles; ii++ ) {    
    pads[ii]->cd();
    SetAttributes( histos[ii], ii );
    histos[ii]->SetMinimum(0.);
    histos[ii]->Draw("HIST");
  }
  theCanvas->Update();
  theCanvas->cd();
  gStyle->SetOptStat(1);

  gStyle->SetOptStat(0);
  pad2->cd();
  for( int ii = 0; ii < nFiles; ii++ ) {    
    if( bnorm ) histos[ii]->SetNormFactor(1.);
    if( ii == 0 ) {
      histos[ii]->Draw("HIST");
    } else {
      histos[ii]->Draw("HISTSames");
    }
  }
  lege->Draw();

  //substitute ": " by "."
  std::string OutFileNamet(OutFileName);
  std::string OutFileNameS;
  int ns = -1;
  int nc = 0;
  for(;;){
    nc = OutFileNamet.find(": ",ns+1);
    OutFileNameS += OutFileNamet.substr(ns+1,nc-ns-1) + ".";
    if( nc == int(std::string::npos) ) break;
    ns = nc+1;
  }

  //substitute "(" and ")" by "."
  for( size_t ii = 0; ii < OutFileNameS.length()-1; ii+=2 ){
    if( OutFileNameS[ii] == '(' || OutFileNameS[ii] == ')') {
      OutFileNameS[ii] = '.';
    }
  }

  theCanvas->Print(std::string("his"+OutFileNameS+".gif").c_str());

  return;

}

//------------------------------------------------------------------------
TH1F* getHisto1( int fileNumber, const char* hnam )
{
  TH1F* his = (TH1F*)(*theFiles[fileNumber]).Get(hnam);
  if( !his ) {
    std::cerr << "!!! ERROR: histo not found " << hnam << std::endl;
  }
  return his;
}


//------------------------------------------------------------------------
void setStyle()
{
//return;

//  gROOT->Reset();
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(1);
  gStyle->SetOptTitle(0);
  //    gStyle->SetOptStat(1111111);
  //  gStyle->SetOptStat(10);
  gStyle->SetOptStat(1);
  //  gSystem->Load("libMuonReco.so");
  //  gStyle->SetStatFont(42);
  //  gStyle->SetTextSize(0.8);
  gStyle->SetTitleFont(42);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadColor(0);
  gStyle->SetLabelFont(42,"x");
  gStyle->SetLabelFont(42,"y");
  gStyle->SetHistFillStyle(1001);
  gStyle->SetHistFillColor(0);
  gStyle->SetOptFit(10);
  //gStyle->SetStatH(0.1);
  //gStyle->SetStatW(0.15);
  //.................... 

}


//------------------------------------------------------------------------
void SetAttributes( TH1F* his, int iflag)
{
  if( iflag == 0 ){
    his->SetLineColor(2);
    his->SetMarkerColor(2);
    his->SetMarkerStyle(20);
    his->SetMarkerSize(0.2);
  } else if(iflag == 1 ){
    his->SetLineColor(4);
    his->SetMarkerColor(4);
    his->SetMarkerStyle(25);
    his->SetMarkerSize(0.2);
  } else if(iflag == 2 ){
    his->SetLineColor(3);
    his->SetMarkerColor(3);
    his->SetMarkerStyle(25);
    his->SetMarkerSize(1.2);
  }

}

