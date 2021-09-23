#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <TF1.h>
#include <TH1D.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include "TH1D.h"
#include <TStyle.h>
#include "TFile.h"
#include "TLine.h"
#include <TArrow.h>
#include <iostream>
#include <TH1F.h>
#include <new>

using namespace std;


Double_t parabolic(Double_t *x, Double_t *par)
{
  return par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
}

double FWHM(int rebin)
{
  //Let's look for the maximum counts value 

	gStyle->SetOptFit();

	TFile* myf = new TFile("img_hist_SOE.root","READ");
	TH1F* myH = (TH1F*) myf->Get("Xprofile"); 
	if (myH == NULL)
	{
		cout<<"TH1D* myH is void!" << endl;
		return;
	}

	myH->Rebin(rebin);

	double RMS = myH->GetRMS();
	int NumOfBins = myH->GetNbinsX();
	double BinWidth = (double) myH->GetBinWidth(NumOfBins);
	double lowx = (double) myH->GetBinLowEdge(1);
	double highx = (double) myH->GetBinLowEdge(NumOfBins)+BinWidth;

	double maximum = 0;
	double maximum_x = 0;
	int maximum_index = 0;

	for(int i=1; i <= NumOfBins; i++)
	{
		if(myH->GetBinContent(i) > maximum)
		{
			maximum = myH ->GetBinContent(i);
			maximum_x = myH ->GetBinCenter(i);
			maximum_index = i;
		}	
	} 

	cout << "maximum_x = " << maximum_x<<endl;
	cout << "maximum = " << maximum<<endl;
	cout << "maximum_index = " << maximum_index<<endl;
	cout << "Number of bins: "<<NumOfBins<<endl;
	cout << "Bin width: "<<BinWidth<<endl;
	cout << "minimum X: "<<lowx<<endl;
	cout << "maximum X: "<<highx<<endl;

	// TH1D *temporal = new TH1D("temporal", "temporal",1000,0,1000);
	TH1D *temporal = new TH1D("temporal", "temporal",NumOfBins,lowx,highx);
	
	temporal->Fill(myH->GetBinCenter(maximum_index-1),myH->GetBinContent(maximum_index-1));
	temporal->Fill(myH->GetBinCenter(maximum_index),myH->GetBinContent(maximum_index));
	temporal->Fill(myH->GetBinCenter(maximum_index+1),myH->GetBinContent(maximum_index+1));
	
	cout << "temporal entries = " << temporal->GetEntries() << endl;
	TCanvas *c1 = new TCanvas();
	temporal->Draw();

	cout << "parabole min and max: " << lowx << " " << highx << endl;
	
	// TF1 *func3 = new TF1("tempfit",parabolic, 0, 1000, 3);
	TF1 *func3 = new TF1("tempfit",parabolic, lowx, highx, 3);
	func3->SetParameters(-500,1,-500);
	temporal->Fit(func3,"","",-1000,1000);
	
	double functionmaximum = -pow(func3->GetParameter(1),2)/(4*func3->GetParameter(0))+(func3->GetParameter(2));
	double halfmaximum = functionmaximum/2;
	cout << "max from parabolic fit = " << functionmaximum << endl;
	cout << "half max = " << halfmaximum << endl;

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

	cout << "x1up = "<<x1up<<endl;
	cout << "x1down = "<<x1down<<endl;
	cout << "x1up_index = "<<x1up_index<<endl;
	cout << "x1down_index = "<<x1down_index<<endl;
	cout << "x1up_index_plus = "<<x1up_index_plus<<endl;
	cout << "x1down_index_plus = "<<x1down_index_plus<<endl;
	cout << "x1up_index_minus = "<<x1up_index_minus<<endl;
	cout << "x1down_index_minus = "<<x1down_index_minus<<endl;
	cout << "x1up_index = "<<x1up_index<<endl;
	cout << "x1down_index = "<<x1down_index<<endl;
	cout << "x2up = "<<x2up<<endl;
	cout << "x2down = "<<x2down<<endl;
	cout << "x2up_index = "<<x2up_index<<endl;
	cout << "x2down_index = "<<x2down_index<<endl;
	cout << "x2up_index_plus = "<<x2up_index_plus<<endl;
	cout << "x2down_index_plus = "<<x2down_index_plus<<endl;
	cout << "x2up_index_minus = "<<x2up_index_minus<<endl;
	cout << "x2down_index_minus = "<<x2down_index_minus<<endl;
	
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
	
	cout << " slope1 = " << slope1 << endl;
	cout << " origin1 = " << origin1 << endl; 
	cout << " slope1_plus = " << slope1_plus << endl;
	cout << " origin1_plus = " << origin1_plus << endl; 
	cout << " slope1_minus = " <<  slope1_minus<< endl;
	cout << " origin1_minus = " <<origin1_minus  << endl; 
	cout << " slope2 = " << slope2 << endl;
	cout << " origin2 = " << origin2 << endl; 
	cout << " slope2_plus = " << slope2_plus << endl;
	cout << " origin2_plus = " << origin2_plus << endl; 
	cout << " slope2_minus = " << slope2_minus << endl;
	cout << " origin2_minus = " << origin2_minus << endl; 
	
	double x1 = (-1/slope1)*((-halfmaximum)+origin1);
	double x1_plus = (-1/slope1_plus)*((-halfmaximum)+origin1_plus);
	double x1_minus = (-1/slope1_minus)*((-halfmaximum)+origin1_minus);
	double x2 = (-1/slope2)*((-halfmaximum)+origin2);
	double x2_plus = (-1/slope2_plus)*((-halfmaximum)+origin2_plus);
	double x2_minus = (-1/slope2_minus)*((-halfmaximum)+origin2_minus);
	TArrow* line3 = new TArrow(x1,halfmaximum,x2,halfmaximum, 0.01, "<|>");
	TLine* line3_max = new TLine(x1_minus,halfmaximum,x2_plus,halfmaximum);
	TLine* line3_min = new TLine(x1_plus,halfmaximum,x2_minus,halfmaximum);

	cout << "FWHM Line points:" << endl;
	cout << "x1: " << x1 << endl;
	cout << "x2: " << x2 << endl;

	double fullwidthhalf = (x2 - x1);
	double fullwidthhalf_max = (x2_plus - x1_minus);
	double fullwidthhalf_min = (x2_minus - x1_plus);

	gROOT->SetStyle("Plain");  
	
	TCanvas* cfwhm = new TCanvas("cfwhm","",500,500);

	myH->GetXaxis()->SetRangeUser(-5,5);
	myH->GetYaxis()->SetTitle("Arbitrary Units");
	myH->GetXaxis()->SetTitle("X [mm]");
	myH->GetXaxis()->SetTitleOffset(1.2);
	myH->SetLineWidth(3);
	myH->SetLineStyle(9);
	myH->SetMarkerColor(46);
	myH->Draw("lp");

	line1->SetLineWidth(2);
	line1->SetLineColor(kRed);

	line2->SetLineWidth(2);
	line2->SetLineColor(kRed);

	line3->SetLineWidth(2);
	line3->SetLineColor(kBlue);
	line3->SetFillColor(kBlue);
	line3->Draw();

	line1_plus->SetLineWidth(1);
	line1_plus->SetLineColor(kRed+2);

	line2_plus->SetLineWidth(1);
	line2_plus->SetLineColor(kRed+2);

	line3_max->SetLineWidth(1);
	line3_max->SetLineColor(kBlue+2);

	line1_minus->SetLineWidth(1);
	line1_minus->SetLineColor(kRed-2);

	line2_minus->SetLineWidth(1);
	line2_minus->SetLineColor(kRed-2);

	line3_min->SetLineWidth(1);
	line3_min->SetLineColor(kBlue-2);

	cout << "RMS: " << RMS << endl;
	cout << "FWHM: " << fullwidthhalf << endl;
	cout << "FWHM_max: " << fullwidthhalf_max << endl;
	cout << "FWHM_min: " << fullwidthhalf_min << endl;
	cout << "Max: " << functionmaximum << endl;
	cout << "Half Max: " << halfmaximum << endl;

	return fullwidthhalf;
}
