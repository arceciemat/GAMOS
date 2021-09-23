#include "TH1F.h"
#include "TCanvas.h"
#include <iostream>
#include <TStyle.h>
#include "TF1.h"
#include <vector>
#include "ROOTUtilities/PlotData/Definitions.C"
#include "ROOTUtilities/GetWordsInLine.C"
#include "ROOTUtilities/GetValue.C"
#include "ROOTUtilities/Verbosity.C"

//------------------------------------------------------------------------
double makePlot1Column(std::string fileName ) 
{
  verbose = 3;
  int iValColumn = 1;
  int hXNBins = 100;
  std::string histoName = "values";

  std::ifstream fin(fileName.c_str());
  std::vector<std::string> wl;
  double hXMin = DBL_MAX;
  double hXMax = -DBL_MAX;
  double sumS = 0.;
  double sumS2 = 0.;
  std::vector<double> values;
  for( ;; ) {
    if( ! GetWordsInLine( fin, wl ) ) break;
    double val = GetValue(wl[iValColumn]);
    hXMin = std::min(val,hXMin);
    hXMax = std::max(val,hXMax);
    values.push_back(val);
    sumS += val;
    sumS2 += val*val;
    if( verbose >= 2 ) std::cout << " READ VAL " << val << " " << sumS << " " << sumS2 << std::endl;
  }

  int nVals = values.size();
  std::cout << "AVERAGE VALUE " << sumS/nVals << " +- " << sqrt((sumS2-sumS*sumS/nVals*nVals)/(nVals)) << " DISPERSION= " << sqrt((sumS2-sumS*sumS/nVals)/(nVals)) << " N_VALS " << nVals << std::endl;

  double hXWid = hXMax-hXMin;
  TH1F* histo = new TH1F(histoName.c_str(), histoName.c_str(), hXNBins, hXMin-0.1*hXWid, hXMax+0.1*hXWid);

  for( int ii = 0; ii < nVals; ii++ ) {
    histo->Fill(values[ii]);
  }

  theCanvas->cd();
  histo->Draw("h");
  theCanvas->Print((histoName+".gif").c_str());
}
