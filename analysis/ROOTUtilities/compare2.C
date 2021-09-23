#if !defined(__CINT__) || defined(__MAKECINT__)
int optSta = 1111111;
#include <TH1.h>
#include <TProfile.h>
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
#include <TKey.h>

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
TProfile* getHisto1Prof(int theFileNumber, const char* hnam );
void compare2His(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm = 0 );
void compare2His( TH1F* his0, TH1F* his1, const char* XName, const char* YName, const char* OutFileName, TH1F* his2 = 0, bool bnorm = 0 );
void compare2HisProf(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm = 0 );
void compare2HisProf( TProfile* his0, TProfile* his1, const char* XName, const char* YName, const char* OutFileName, TProfile* his2 = 0, bool bnorm = 0 );
void SetAttributes( TH1F* his, int iflag);
void SetAttributes( TProfile* his, int iflag);

char * theGlobalTitle;
std::vector<char*> theSampleTitle;
int nFiles;

//------------------------------------------------------------------------
void InitComparison(const char* filename1, const char* filename2, const char* filename3 = "")  
{

  theOptLogy = 0;  
  setStyle();

  theFiles.push_back( new TFile(filename1) );
  theFiles.push_back( new TFile(filename2) );
  nFiles = 2;
  if( filename3 != std::string("") ){
    theFiles.push_back( new TFile(filename3) );
    nFiles++;
  }

  //  theFiles[0]->ls();
  theCanvas = new TCanvas("dd");
  theCanvas->SetCanvasSize(2000,2000);
  theCanvas->cd();

}


//------------------------------------------------------------------------
void compare2His(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm )
{
  TH1F* his0 = getHisto1(0, hnam);
  TH1F* his1 = getHisto1(1, hnam);
  TH1F* his2 = 0;
  if( nFiles == 3 ){
    his2 = getHisto1(2, hnam); // histo from third file
  }
  if( !his0 ) {
    std::cerr << "!!! ERROR: histo not found in first file " << std::endl;
    return;
  }
  if( !his1 ) {
    std::cerr << "!!! ERROR: histo not found in second file " << std::endl;
    return;
  }
  compare2His( his0, his1, XName, YName, OutFileName, his2, bnorm );
}


//------------------------------------------------------------------------
void compare2His( TH1F* his0, TH1F* his1, const char* XName, const char* YName, const char* OutFileName, TH1F* his2, bool bnorm )
{
  // First the attributes ....
  SetAttributes( his0, 0 );
  SetAttributes( his1, 1 );
  if( nFiles == 3 ){
    SetAttributes( his2, 2 );
  }

  his0->SetXTitle(XName);
  his0->SetYTitle(YName);

  TLegend* lege = new TLegend(0.75,0.75,0.98,1.0);
  lege->SetTextAlign(3);
  lege->SetTextSize(0.1);
  lege->SetTextColor(1);
  lege->AddEntry(his0,theSampleTitle[0],"P");
  lege->AddEntry(his1,theSampleTitle[1],"P");
  if( nFiles == 3 ){
    lege->AddEntry(his2,theSampleTitle[2],"P");
  }

  TPaveLabel* tit_eta_glo = new TPaveLabel(0.0,0.0,1.0,1.0,theGlobalTitle);
  tit_eta_glo->SetTextColor(1);

  theCanvas->cd();

  TPad* pada11 = new TPad("pada","Pad a",0.,0.5,0.5,1.);
  TPad* pada12 = new TPad("pada","Pad a",0.5,0.5,1.,1.);
  TPad* pada2 = new TPad("pada","Pad a",0.,0.,1.0,0.5);
  TPad* padb1 = new TPad("padb","Pad b",0.,0.,1.,1.);

  gStyle->SetOptStat(optSta);
  pada11->SetLogy(theOptLogy);
  pada12->SetLogy(theOptLogy);
  pada2->SetLogy(theOptLogy);
  padb1->SetLogy(theOptLogy);

  pada11->Draw();
  pada12->Draw();
 
  pada11->cd();
  if( theOptLogy ) {
    his0->SetMinimum(1.e-10);
    his1->SetMinimum(1.e-10);
  } else {
    his0->SetMinimum(0.);
    his1->SetMinimum(0.);
  }
  gStyle->SetOptStat(111111);
  his0->Draw("HIST");

  pada12->cd();
  gStyle->SetOptStat(optSta);
  his1->Draw("HIST");

  theCanvas->Update();
  theCanvas->cd();
  gStyle->SetOptStat(optSta);
  pada2->Draw();
  pada2->cd();
  TH1F* his0d;
  TH1F* his1d;
  if( bnorm )  {
    his0->SetNormFactor(1.);
    his1->SetNormFactor(1.);
    his0d = new TH1F(*his0);
    his1d = new TH1F(*his1);
  }
  gStyle->SetOptStat(optSta);
  his0->Draw("HIST");
  his1->Draw("HISTSames");
  // his1->Draw("E");
  if( nFiles == 3 ){
    his2->Draw("Sames");
  }
  //  lege->Draw();

  //substitute ": " by "."
  std::string OutFileNamet(OutFileName);
  std::string OutFileNameS;
  int ns = -1;
  int nc = 0;
  for(;;){
    nc = OutFileNamet.find(": ",ns+1);
    OutFileNameS += OutFileNamet.substr(ns+1,nc-ns-1);
    if( nc == int(std::string::npos) ) break;
    OutFileNameS += ".";
    ns = nc+1;
  }

  //substitute "(" and ")" by "."
  for( size_t ii = 0; ii < OutFileNameS.length()-1; ii+=2 ){
    if( OutFileNameS[ii] == '(' || OutFileNameS[ii] == ')') {
      OutFileNameS[ii] = '.';
    }
  }

  std::replace( OutFileNameS.begin(), OutFileNameS.end(), '/', '_'); 
  theCanvas->Print(std::string("his"+OutFileNameS+".gif").c_str());

  // print histogram division
  theCanvas->cd();
  gStyle->SetOptStat(0);
  padb1->Draw();

  padb1->cd();
  TH1F* hisd = new TH1F(*his0d);
  float rat = (his1->Integral() + his1->GetBinContent(0)
	       +his1->GetBinContent(his1->GetNbinsX()+1) )/ 
	       (his0->Integral()+his0->GetBinContent(0)+his0->GetBinContent(his0->GetNbinsX()+1) );

  if( his0->GetEntries() != 0 && his1->GetEntries() != 0 ){
    hisd->Divide(his1d);
    int nbinsNo0 = 0;
    for (int ii = 0; ii <= hisd->GetNbinsX()+1; ii++ ) {
      if( his0->GetBinContent(ii) != 0. && his1->GetBinContent(ii) != 0. ){
	nbinsNo0++;
	//t	hisd->SetBinError( ii, sqrt(1./his0->GetBinContent(ii) + 1./his1->GetBinContent(ii) ) );
	//	if( ii < 10 ) std::cout << ii << " val " << his0->GetBinContent(ii) << " " << his1->GetBinContent(ii) <<" +- " << sqrt(1./his0->GetBinContent(ii) + 1./his1->GetBinContent(ii) ) << std::endl;	
	float val = his0d->GetBinContent(ii)/ his1d->GetBinContent(ii);
	float error = val*sqrt( pow(his0d->GetBinError(ii)/his0d->GetBinContent(ii),2) + pow(his1d->GetBinError(ii)/ his1d->GetBinContent(ii),2) );
	if( bnorm ) {
	  val *= rat;
	  error *= rat;
	}
	hisd->SetBinContent(ii, val );
	//	hisd->SetBinError( ii, error );
      }

    }
    //    if(bnorm) hisd->SetNormFactor(nbinsNo0);

    hisd->SetMaximum(2.);
    hisd->Draw("");
  }

  TLine* line = new TLine(hisd->GetXaxis()->GetXmin(),1.,hisd->GetXaxis()->GetXmax(),1.);
  line->Draw();

  std::replace( OutFileNameS.begin(), OutFileNameS.end(), '/', '_'); 
  theCanvas->Print(std::string("his"+OutFileNameS+"_div.gif").c_str());

  theCanvas->cd();

}

//------------------------------------------------------------------------
void compare2HisProf(const char* hnam, const char* XName, const char* YName, const char* OutFileName, bool bnorm )
{
  TProfile* his0 = getHisto1Prof(0, hnam);
  TProfile* his1 = getHisto1Prof(1, hnam);
  TProfile* his2 = 0;
  if( nFiles == 3 ){
    his2 = getHisto1Prof(2, hnam); // histo from third file
  }
  if( !his0 ) {
    std::cerr << "!!! ERROR: histo not found in first file " << std::endl;
    return;
  }
  if( !his1 ) {
    std::cerr << "!!! ERROR: histo not found in second file " << std::endl;
    return;
  }
  compare2HisProf( his0, his1, XName, YName, OutFileName, his2, bnorm );
}


//------------------------------------------------------------------------
void compare2HisProf( TProfile* his0, TProfile* his1, const char* XName, const char* YName, const char* OutFileName, TProfile* his2, bool bnorm )
{
  // First the attributes ....
  SetAttributes( his0, 0 );
  SetAttributes( his1, 1 );
  if( nFiles == 3 ){
    SetAttributes( his2, 2 );
  }

  his0->SetXTitle(XName);
  his0->SetYTitle(YName);

  TLegend* lege = new TLegend(0.75,0.75,0.98,1.0);
  lege->SetTextAlign(3);
  lege->SetTextSize(0.1);
  lege->SetTextColor(1);
  lege->AddEntry(his0,theSampleTitle[0],"P");
  lege->AddEntry(his1,theSampleTitle[1],"P");
  if( nFiles == 3 ){
    lege->AddEntry(his2,theSampleTitle[2],"P");
  }

  TPaveLabel* tit_eta_glo = new TPaveLabel(0.0,0.0,1.0,1.0,theGlobalTitle);
  tit_eta_glo->SetTextColor(1);

  theCanvas->cd();

  TPad* pada11 = new TPad("pada","Pad a",0.,0.5,0.5,1.);
  TPad* pada12 = new TPad("pada","Pad a",0.5,0.5,1.,1.);
  TPad* pada2 = new TPad("pada","Pad a",0.,0.,1.0,0.5);
  TPad* padb1 = new TPad("padb","Pad b",0.,0.,1.,1.);

  gStyle->SetOptStat(optSta);
  pada11->SetLogy(theOptLogy);
  pada12->SetLogy(theOptLogy);
  pada2->SetLogy(theOptLogy);
  padb1->SetLogy(theOptLogy);

  pada11->Draw();
  pada12->Draw();
 
  pada11->cd();
  if( theOptLogy ) {
    his0->SetMinimum(1.e-10);

    his1->SetMinimum(1.e-10);
  } else {
    his0->SetMinimum(0.);
    his1->SetMinimum(0.);
  }
  his0->Draw("HIST");

  pada12->cd();
  his1->Draw("HIST");

  theCanvas->Update();
  theCanvas->cd();
  gStyle->SetOptStat(optSta);
  pada2->Draw();
  pada2->cd();
  gStyle->SetOptStat(optSta);
  TProfile* his0d;
  TProfile* his1d;
  if( bnorm )  {
    his0->SetNormFactor(1.);
    his1->SetNormFactor(1.);
    his0d = new TProfile(*his0);
    his1d = new TProfile(*his1);
  }
  gStyle->SetOptStat(optSta);
  his0->Draw("HIST");
  his1->Draw("HISTSames");
  // his1->Draw("E");
  if( nFiles == 3 ){
    his2->Draw("Sames");
  }
  //  lege->Draw();

  //substitute ": " by "."
  std::string OutFileNamet(OutFileName);
  std::string OutFileNameS;
  int ns = -1;
  int nc = 0;
  for(;;){
    nc = OutFileNamet.find(": ",ns+1);
    OutFileNameS += OutFileNamet.substr(ns+1,nc-ns-1);
    if( nc == int(std::string::npos) ) break;
    OutFileNameS += ".";
    ns = nc+1;
  }

  //substitute "(" and ")" by "."
  for( size_t ii = 0; ii < OutFileNameS.length()-1; ii+=2 ){
    if( OutFileNameS[ii] == '(' || OutFileNameS[ii] == ')') {
      OutFileNameS[ii] = '.';
    }
  }

  std::replace( OutFileNameS.begin(), OutFileNameS.end(), '/', '_'); 
  theCanvas->Print(std::string("his"+OutFileNameS+".gif").c_str());

  return;

  // print histogram division
  theCanvas->cd();
  gStyle->SetOptStat(0);
  padb1->Draw();

  padb1->cd();
  TProfile* hisd = new TProfile(*his0d);
  float rat = (his1->Integral() + his1->GetBinContent(0)
	       +his1->GetBinContent(his1->GetNbinsX()+1) )/ 
	       (his0->Integral()+his0->GetBinContent(0)+his0->GetBinContent(his0->GetNbinsX()+1) );

  if( his0->GetEntries() != 0 && his1->GetEntries() != 0 ){
    hisd->Divide(his1d);
    int nbinsNo0 = 0;
    for (int ii = 0; ii <= hisd->GetNbinsX()+1; ii++ ) {
      if( his0->GetBinContent(ii) != 0. && his1->GetBinContent(ii) != 0. ){
	nbinsNo0++;
	//t	hisd->SetBinError( ii, sqrt(1./his0->GetBinContent(ii) + 1./his1->GetBinContent(ii) ) );
	//	if( ii < 10 ) std::cout << ii << " val " << his0->GetBinContent(ii) << " " << his1->GetBinContent(ii) <<" +- " << sqrt(1./his0->GetBinContent(ii) + 1./his1->GetBinContent(ii) ) << std::endl;	
	float val = his0d->GetBinContent(ii)/ his1d->GetBinContent(ii);
	float error = val*sqrt( pow(his0d->GetBinError(ii)/his0d->GetBinContent(ii),2) + pow(his1d->GetBinError(ii)/ his1d->GetBinContent(ii),2) );
	if( bnorm ) {
	  val *= rat;
	  error *= rat;
	}
	hisd->SetBinContent(ii, val );
	//	hisd->SetBinError( ii, error );
      }

    }
    //    if(bnorm) hisd->SetNormFactor(nbinsNo0);

    hisd->SetMaximum(2.);
    hisd->Draw("");
  }

  TLine* line = new TLine(hisd->GetXaxis()->GetXmin(),1.,hisd->GetXaxis()->GetXmax(),1.);
  line->Draw();

  std::replace( OutFileNameS.begin(), OutFileNameS.end(), '/', '_'); 
  theCanvas->Print(std::string("his"+OutFileNameS+"_div.gif").c_str());

  theCanvas->cd();

}

//------------------------------------------------------------------------
TH1F* getHisto1( int fileNumber, const char* hnam )
{
  TH1F* his = 0;
  TIter ListOfHistograms( theFiles[fileNumber]->GetListOfKeys() );
  TKey *HistoKey;  
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    std::string hisname = HistoKey->GetName()+std::string(";1");
    TObject *obj = HistoKey->ReadObj();
    if( dynamic_cast<TH1F*>(obj) ) {
      std::cout << " getHisto1 COMPARE " << std::string(hnam) << " == " << hisname  << std::endl; //GDEB
      if( std::string(hnam) == hisname ||
	 std::string(hnam)+";1" == hisname ) {
	his = dynamic_cast<TH1F*>(obj);
	std::cout << " getHisto1 HISTO FOUND " << his << " " << his->GetName() << std::endl; //GDEB
      }
    }
  }
  //  TH1F* his = (TH1F*)(*theFiles[fileNumber]).Get(hnam);
  if( !his ) {
    std::cerr << "!!! ERROR: histo not found " << hnam << std::endl;
  }
  return his;
}

//------------------------------------------------------------------------
TProfile* getHisto1Prof( int fileNumber, const char* hnam )
{
  TProfile* his = (TProfile*)(*theFiles[fileNumber]).Get(hnam);
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
  //    gStyle->SetOptStat(optSta);
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


//------------------------------------------------------------------------
void SetAttributes( TProfile* his, int iflag)
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
