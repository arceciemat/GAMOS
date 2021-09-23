#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <Rtypes.h>
#include <TGraph.h>
#include <TAttMarker.h>
#include <TChain.h>
#include <TH2F.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TNamed.h>
#include <TFriendElement.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TFormula.h>
#include <TNamed.h>
#include <TFile.h>
#include "Riostream.h"
#include "limits.h"
#include <float.h>  
#include <stdlib.h>


using namespace std;

TFile* myHfile = NULL;
TH1D* myH = NULL;
Int_t Max = 0, maxX = 0, maxY = 0;  


void myOpen()
{

  myHfile =  new TFile("img_hist_SOE.root","READ");  
  cout << endl << "*** img_hist_SOE.root opened ***" << endl;

}

void myGet(TString myHname)
{
  if(myHfile == NULL){ 
    myHfile =  new TFile("img_hist_SOE.root","READ");  
    cout << endl << "*** img_hist_SOE.root opened ***" << endl;
  }
  myH = (TH1D*) myHfile->Get(myHname);
  cout << endl << "**************************************"; 
  cout << endl << "Histogram: " << myHname << " saved on pointer: TH1D* myH" << endl;
}



Double_t parabolic(Double_t *x, Double_t *par){
  
  //cout << "x max pos: " << par[1]/(-2.0*par[0]) << endl;

  return par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
}


void FWHM(int rebin)
{

  if(myH == NULL){
    cout<<"TH1D* myH is void!" << endl;
    return;
      }

  myH->Rebin(rebin);

  double RMS = myH->GetRMS();
  int NumOfBins = myH->GetNbinsX();
  double BinWitdh = (double) myH->GetBinWidth(NumOfBins);
  double lowx = (double) myH->GetBinLowEdge(1);
  double highx = (double) myH->GetBinLowEdge(NumOfBins)+BinWitdh;

  double maximum = 0;
  double maximum_x = 0;
  int maximum_index = 0;
  



  for(int i=1;i<NumOfBins;i++)
    {
      if(myH->GetBinContent(i)>maximum)
	{
	  maximum = myH ->GetBinContent(i);
	  maximum_x = myH ->GetBinCenter(i);
	  maximum_index = i;
	}

    } 
  cout<<"maximum_x = "<< maximum_x<<endl;
  cout<<"maximum = "<< maximum<<endl;
  cout<<"maximum_index = "<< maximum_index<<endl;
  cout<<"Number of bins: "<<NumOfBins<<endl;
  cout<<"Bin witdh: "<<BinWitdh<<endl;
  cout<<"minimum X: "<<lowx<<endl;
  cout<<"maximum X: "<<highx<<endl;
  

  TH1D *temporal = new TH1D("temporal", "temporal",NumOfBins,lowx,highx);
  
  temporal->Fill(myH->GetBinCenter(maximum_index-1),myH->GetBinContent(maximum_index-1));
  temporal->Fill(myH->GetBinCenter(maximum_index),myH->GetBinContent(maximum_index));
  temporal->Fill(myH->GetBinCenter(maximum_index+1),myH->GetBinContent(maximum_index+1));

  cout<<"temporal entries = "<<temporal->GetEntries()<<endl;
  TCanvas *c1 = new TCanvas();
  temporal->Draw();

  TF1 *func3 = new TF1("tempfit",parabolic,lowx,highx,3);
  func3->SetParameters(-500,1,-500);
  temporal->Fit(func3,"","",-1000,1000);

  double functionmaximum = -pow(func3->GetParameter(1),2)/(4*func3->GetParameter(0))+(func3->GetParameter(2));
  double halfmaximum = functionmaximum/2;
  cout<<"max from parabolic fit = "<<functionmaximum<<endl;
  cout<<"half max = "<<halfmaximum<<endl;

  double x1up = 0;
  double x1down = 0;
  double x2up = 0;
  double x2down = 0;
  double x1up_index = 0;
  double x1down_index = 0;
  double x1up_index_plus = 0;
  double x1down_index_plus = 0;
  double x1up_index_minus = 0;
  double x1down_index_minus = 0;
  double x2up_index = 0;
  double x2down_index = 0;  
  double x2up_index_plus = 0;
  double x2down_index_plus = 0;
  double x2up_index_minus = 0;
  double x2down_index_minus = 0;


  for(int i=maximum_index;i>0;i--)
    {
      if(myH->GetBinContent(i)<halfmaximum)
	{
	  x1up = myH->GetBinContent(i+1);
	  x1down = myH ->GetBinContent(i);
	  x1up_index = myH->GetBinCenter(i+1);
	  x1down_index = myH->GetBinCenter(i);
	  x1up_index_plus = myH->GetBinLowEdge(i+2);
	  x1down_index_plus = myH->GetBinLowEdge(i+1);
	  x1up_index_minus = myH->GetBinLowEdge(i+1);
	  x1down_index_minus = myH->GetBinLowEdge(i);
	  break;
	}
    } 


  for( int i=maximum_index;i<NumOfBins;i++)
    {
      if(myH->GetBinContent(i)<halfmaximum)
	{
	  x2up = myH->GetBinContent(i-1);
	  x2down = myH ->GetBinContent(i);
	  x2up_index = myH->GetBinCenter(i-1);
	  x2down_index = myH->GetBinCenter(i);
	  x2up_index_plus = myH->GetBinLowEdge(i);
	  x2down_index_plus = myH->GetBinLowEdge(i+1);
	  x2up_index_minus = myH->GetBinLowEdge(i-1);
	  x2down_index_minus = myH->GetBinLowEdge(i);
	  break;
	}
    }

  cout<<"x1up = "<<x1up<<endl;
  cout<<"x1down = "<<x1down<<endl;
  cout<<"x1up_index = "<<x1up_index<<endl;
  cout<<"x1down_index = "<<x1down_index<<endl;
  cout<<"x1up_index_plus = "<<x1up_index_plus<<endl;
  cout<<"x1down_index_plus = "<<x1down_index_plus<<endl;
  cout<<"x1up_index_minus = "<<x1up_index_minus<<endl;
  cout<<"x1down_index_minus = "<<x1down_index_minus<<endl;
  cout<<"x1up_index = "<<x1up_index<<endl;
  cout<<"x1down_index = "<<x1down_index<<endl;
  cout<<"x2up = "<<x2up<<endl;
  cout<<"x2down = "<<x2down<<endl;
  cout<<"x2up_index = "<<x2up_index<<endl;
  cout<<"x2down_index = "<<x2down_index<<endl;
  cout<<"x2up_index_plus = "<<x2up_index_plus<<endl;
  cout<<"x2down_index_plus = "<<x2down_index_plus<<endl;
  cout<<"x2up_index_minus = "<<x2up_index_minus<<endl;
  cout<<"x2down_index_minus = "<<x2down_index_minus<<endl;

  TLine* line1 = new TLine(x1down_index,x1down,x1up_index,x1up);
  TLine* line1_plus = new TLine(x1down_index_plus,x1down,x1up_index_plus,x1up);
  TLine* line1_minus = new TLine(x1down_index_minus,x1down,x1up_index_minus,x1up);
  TLine* line2 = new TLine(x2down_index,x2down,x2up_index,x2up);
  TLine* line2_plus = new TLine(x2down_index_plus,x2down,x2up_index_plus,x2up);
  TLine* line2_minus = new TLine(x2down_index_minus,x2down,x2up_index_minus,x2up);

  double slope1 =(x1up - x1down)/ (x1up_index - x1down_index);
  double origin1 = x1down - (slope1*x1down_index);
  double slope1_plus =(x1up - x1down)/ (x1up_index_plus - x1down_index_plus);
  double origin1_plus = x1down - (slope1_plus*x1down_index_plus);
  double slope1_minus =(x1up - x1down)/ (x1up_index_minus - x1down_index_minus);
  double origin1_minus = x1down - (slope1_minus*x1down_index_minus);
  double slope2 =(x2up - x2down)/(x2up_index - x2down_index);
  double origin2 = x2down - (slope2*x2down_index);
  double slope2_plus =(x2up - x2down)/ (x2up_index_plus - x2down_index_plus);
  double origin2_plus = x2down - (slope2_plus*x2down_index_plus);
  double slope2_minus =(x2up - x2down)/ (x2up_index_minus - x2down_index_minus);
  double origin2_minus = x2down - (slope2_minus*x2down_index_minus);

  cout<<" slope1 = " << slope1 << endl;
  cout<<" origin1 = " << origin1 << endl; 
  cout<<" slope1_plus = " << slope1_plus << endl;
  cout<<" origin1_plus = " << origin1_plus << endl; 
  cout<<" slope1_minus = " <<  slope1_minus<< endl;
  cout<<" origin1_minus = " <<origin1_minus  << endl; 
  cout<<" slope2 = " << slope2 << endl;
  cout<<" origin2 = " << origin2 << endl; 
  cout<<" slope2_plus = " << slope2_plus << endl;
  cout<<" origin2_plus = " << origin2_plus << endl; 
  cout<<" slope2_minus = " << slope2_minus << endl;
  cout<<" origin2_minus = " << origin2_minus << endl; 


  double x1 = (-1/slope1)*((-halfmaximum)+origin1);
  double x1_plus = (-1/slope1_plus)*((-halfmaximum)+origin1_plus);
  double x1_minus = (-1/slope1_minus)*((-halfmaximum)+origin1_minus);
  double x2 = (-1/slope2)*((-halfmaximum)+origin2);
  double x2_plus = (-1/slope2_plus)*((-halfmaximum)+origin2_plus);
  double x2_minus = (-1/slope2_minus)*((-halfmaximum)+origin2_minus);
  TLine* line3 = new TLine(x1,halfmaximum,x2,halfmaximum);
  TLine* line3_max = new TLine(x1_minus,halfmaximum,x2_plus,halfmaximum);
  TLine* line3_min = new TLine(x1_plus,halfmaximum,x2_minus,halfmaximum);



  double fullwidthhalf = (x2 - x1);
  double fullwidthhalf_max = (x2_plus - x1_minus);
  double fullwidthhalf_min = (x2_minus - x1_plus);
  
  gROOT->SetStyle("Plain");  

  TCanvas *cfwhm = new TCanvas("cfwhm","",500,500);
  myH->Draw("e");
  line1->SetLineWidth(2);
  line1->SetLineColor(kRed);
  line1->Draw("same");
  line2->SetLineWidth(2);
  line2->SetLineColor(kRed);
  line2->Draw("same");
  line3->SetLineWidth(2);
  line3->SetLineColor(kBlue);
  line3->Draw("same");
  line1_plus->SetLineWidth(1);
  line1_plus->SetLineColor(kRed+2);
  line1_plus->Draw("same");
  line2_plus->SetLineWidth(1);
  line2_plus->SetLineColor(kRed+2);
  line2_plus->Draw("same");
  line3_max->SetLineWidth(1);
  line3_max->SetLineColor(kBlue+2);
  line3_max->Draw("same");
  line1_minus->SetLineWidth(1);
  line1_minus->SetLineColor(kRed-2);
  line1_minus->Draw("same");
  line2_minus->SetLineWidth(1);
  line2_minus->SetLineColor(kRed-2);
  line2_minus->Draw("same");
  line3_min->SetLineWidth(1);
  line3_min->SetLineColor(kBlue-2);
  line3_min->Draw("same");
  //func3->Draw("same");
  cout << "RMS: " << RMS << endl;
  cout << "FWHM: " << fullwidthhalf << endl;
  cout << "FWHM_max: " << fullwidthhalf_max << endl;
  cout << "FWHM_min: " << fullwidthhalf_min << endl;
  cout << "Max: " << functionmaximum << endl;
  cout << "Half Max: " << halfmaximum << endl;
}


void FWTM(int rebin)
{

  if(myH == NULL){
    cout<<"TH1D* myH is void!" << endl;
    return;
      }

  myH->Rebin(rebin);

  double RMS = myH->GetRMS();
  int NumOfBins = myH->GetNbinsX();
  double BinWitdh = (double) myH->GetBinWidth(NumOfBins);
  double lowx = (double) myH->GetBinLowEdge(1);
  double highx = (double) myH->GetBinLowEdge(NumOfBins)+BinWitdh;

  double maximum = 0;
  double maximum_x = 0;
  int maximum_index = 0;
  



  for(int i=1;i<NumOfBins;i++)
    {
      if(myH->GetBinContent(i)>maximum)
	{
	  maximum = myH ->GetBinContent(i);
	  maximum_x = myH ->GetBinCenter(i);
	  maximum_index = i;
	}

    } 
  cout<<"maximum_x = "<< maximum_x<<endl;
  cout<<"maximum = "<< maximum<<endl;
  cout<<"maximum_index = "<< maximum_index<<endl;
  cout<<"Number of bins: "<<NumOfBins<<endl;
  cout<<"Bin witdh: "<<BinWitdh<<endl;
  cout<<"minimum X: "<<lowx<<endl;
  cout<<"maximum X: "<<highx<<endl;
  

  TH1D *temporal = new TH1D("temporal", "temporal",NumOfBins,lowx,highx);
  
  temporal->Fill(myH->GetBinCenter(maximum_index-1),myH->GetBinContent(maximum_index-1));
  temporal->Fill(myH->GetBinCenter(maximum_index),myH->GetBinContent(maximum_index));
  temporal->Fill(myH->GetBinCenter(maximum_index+1),myH->GetBinContent(maximum_index+1));

  cout<<"temporal entries = "<<temporal->GetEntries()<<endl;
  TCanvas *c1 = new TCanvas();
  temporal->Draw();

  TF1 *func3 = new TF1("tempfit",parabolic,lowx,highx,3);
  func3->SetParameters(-500,1,-500);
  temporal->Fit(func3,"","",-1000,1000);

  double functionmaximum = -pow(func3->GetParameter(1),2)/(4*func3->GetParameter(0))+(func3->GetParameter(2));
  double tenthmaximum = functionmaximum/10.;
  cout<<"max from parabolic fit = "<<functionmaximum<<endl;
  cout<<"tenth max = "<<tenthmaximum<<endl;

  double x1up = 0;
  double x1down = 0;
  double x2up = 0;
  double x2down = 0;
  double x1up_index = 0;
  double x1down_index = 0;
  double x1up_index_plus = 0;
  double x1down_index_plus = 0;
  double x1up_index_minus = 0;
  double x1down_index_minus = 0;
  double x2up_index = 0;
  double x2down_index = 0;  
  double x2up_index_plus = 0;
  double x2down_index_plus = 0;
  double x2up_index_minus = 0;
  double x2down_index_minus = 0;


  for(int i=maximum_index;i>0;i--)
    {
      if(myH->GetBinContent(i)<tenthmaximum)
	{
	  x1up = myH->GetBinContent(i+1);
	  x1down = myH ->GetBinContent(i);
	  x1up_index = myH->GetBinCenter(i+1);
	  x1down_index = myH->GetBinCenter(i);
	  x1up_index_plus = myH->GetBinLowEdge(i+2);
	  x1down_index_plus = myH->GetBinLowEdge(i+1);
	  x1up_index_minus = myH->GetBinLowEdge(i+1);
	  x1down_index_minus = myH->GetBinLowEdge(i);
	  break;
	}
    } 


  for( int i=maximum_index;i<NumOfBins;i++)
    {
      if(myH->GetBinContent(i)<tenthmaximum)
	{
	  x2up = myH->GetBinContent(i-1);
	  x2down = myH ->GetBinContent(i);
	  x2up_index = myH->GetBinCenter(i-1);
	  x2down_index = myH->GetBinCenter(i);
	  x2up_index_plus = myH->GetBinLowEdge(i);
	  x2down_index_plus = myH->GetBinLowEdge(i+1);
	  x2up_index_minus = myH->GetBinLowEdge(i-1);
	  x2down_index_minus = myH->GetBinLowEdge(i);
	  break;
	}
    }

  cout<<"x1up = "<<x1up<<endl;
  cout<<"x1down = "<<x1down<<endl;
  cout<<"x1up_index = "<<x1up_index<<endl;
  cout<<"x1down_index = "<<x1down_index<<endl;
  cout<<"x1up_index_plus = "<<x1up_index_plus<<endl;
  cout<<"x1down_index_plus = "<<x1down_index_plus<<endl;
  cout<<"x1up_index_minus = "<<x1up_index_minus<<endl;
  cout<<"x1down_index_minus = "<<x1down_index_minus<<endl;
  cout<<"x1up_index = "<<x1up_index<<endl;
  cout<<"x1down_index = "<<x1down_index<<endl;
  cout<<"x2up = "<<x2up<<endl;
  cout<<"x2down = "<<x2down<<endl;
  cout<<"x2up_index = "<<x2up_index<<endl;
  cout<<"x2down_index = "<<x2down_index<<endl;
  cout<<"x2up_index_plus = "<<x2up_index_plus<<endl;
  cout<<"x2down_index_plus = "<<x2down_index_plus<<endl;
  cout<<"x2up_index_minus = "<<x2up_index_minus<<endl;
  cout<<"x2down_index_minus = "<<x2down_index_minus<<endl;

  TLine* line1 = new TLine(x1down_index,x1down,x1up_index,x1up);
  TLine* line1_plus = new TLine(x1down_index_plus,x1down,x1up_index_plus,x1up);
  TLine* line1_minus = new TLine(x1down_index_minus,x1down,x1up_index_minus,x1up);
  TLine* line2 = new TLine(x2down_index,x2down,x2up_index,x2up);
  TLine* line2_plus = new TLine(x2down_index_plus,x2down,x2up_index_plus,x2up);
  TLine* line2_minus = new TLine(x2down_index_minus,x2down,x2up_index_minus,x2up);

  double slope1 =(x1up - x1down)/ (x1up_index - x1down_index);
  double origin1 = x1down - (slope1*x1down_index);
  double slope1_plus =(x1up - x1down)/ (x1up_index_plus - x1down_index_plus);
  double origin1_plus = x1down - (slope1_plus*x1down_index_plus);
  double slope1_minus =(x1up - x1down)/ (x1up_index_minus - x1down_index_minus);
  double origin1_minus = x1down - (slope1_minus*x1down_index_minus);
  double slope2 =(x2up - x2down)/(x2up_index - x2down_index);
  double origin2 = x2down - (slope2*x2down_index);
  double slope2_plus =(x2up - x2down)/ (x2up_index_plus - x2down_index_plus);
  double origin2_plus = x2down - (slope2_plus*x2down_index_plus);
  double slope2_minus =(x2up - x2down)/ (x2up_index_minus - x2down_index_minus);
  double origin2_minus = x2down - (slope2_minus*x2down_index_minus);

  cout<<" slope1 = " << slope1 << endl;
  cout<<" origin1 = " << origin1 << endl; 
  cout<<" slope1_plus = " << slope1_plus << endl;
  cout<<" origin1_plus = " << origin1_plus << endl; 
  cout<<" slope1_minus = " <<  slope1_minus<< endl;
  cout<<" origin1_minus = " <<origin1_minus  << endl; 
  cout<<" slope2 = " << slope2 << endl;
  cout<<" origin2 = " << origin2 << endl; 
  cout<<" slope2_plus = " << slope2_plus << endl;
  cout<<" origin2_plus = " << origin2_plus << endl; 
  cout<<" slope2_minus = " << slope2_minus << endl;
  cout<<" origin2_minus = " << origin2_minus << endl; 


  double x1 = (-1/slope1)*((-tenthmaximum)+origin1);
  double x1_plus = (-1/slope1_plus)*((-tenthmaximum)+origin1_plus);
  double x1_minus = (-1/slope1_minus)*((-tenthmaximum)+origin1_minus);
  double x2 = (-1/slope2)*((-tenthmaximum)+origin2);
  double x2_plus = (-1/slope2_plus)*((-tenthmaximum)+origin2_plus);
  double x2_minus = (-1/slope2_minus)*((-tenthmaximum)+origin2_minus);
  TLine* line3 = new TLine(x1,tenthmaximum,x2,tenthmaximum);
  TLine* line3_max = new TLine(x1_minus,tenthmaximum,x2_plus,tenthmaximum);
  TLine* line3_min = new TLine(x1_plus,tenthmaximum,x2_minus,tenthmaximum);



  double fullwidthtenth = (x2 - x1);
  double fullwidthtenth_max = (x2_plus - x1_minus);
  double fullwidthtenth_min = (x2_minus - x1_plus);
  
  gROOT->SetStyle("Plain");  

  TCanvas *cfwhm = new TCanvas("cfwhm","",500,500);
  myH->Draw("e");
  line1->SetLineWidth(2);
  line1->SetLineColor(kRed);
  line1->Draw("same");
  line2->SetLineWidth(2);
  line2->SetLineColor(kRed);
  line2->Draw("same");
  line3->SetLineWidth(2);
  line3->SetLineColor(kBlue);
  line3->Draw("same");
  line1_plus->SetLineWidth(1);
  line1_plus->SetLineColor(kRed+2);
  line1_plus->Draw("same");
  line2_plus->SetLineWidth(1);
  line2_plus->SetLineColor(kRed+2);
  line2_plus->Draw("same");
  line3_max->SetLineWidth(1);
  line3_max->SetLineColor(kBlue+2);
  line3_max->Draw("same");
  line1_minus->SetLineWidth(1);
  line1_minus->SetLineColor(kRed-2);
  line1_minus->Draw("same");
  line2_minus->SetLineWidth(1);
  line2_minus->SetLineColor(kRed-2);
  line2_minus->Draw("same");
  line3_min->SetLineWidth(1);
  line3_min->SetLineColor(kBlue-2);
  line3_min->Draw("same");
  //func3->Draw("same");
  cout << "RMS: " << RMS << endl;
  cout << "FWTM: " << fullwidthtenth << endl;
  cout << "FWTM_max: " << fullwidthtenth_max << endl;
  cout << "FWTM_min: " << fullwidthtenth_min << endl;
  cout << "Max: " << functionmaximum << endl;
  cout << "Tenth Max: " << tenthmaximum << endl;
}
