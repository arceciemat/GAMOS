#include <cfloat>
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TSystem.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <map>

int SHIFT;
void testMeanErr()
{
  TCanvas* theCanvas = new TCanvas;
  TFile* file = new TFile("testMeanErr.root","RECREATE");
  TH1F* his1 = new TH1F("testMeanErr","testMeanErr",3,0.5,3.5);
  double xmin = his1->GetXaxis()->GetXmin();
  double step = his1->GetBinWidth(1);
  his1->Fill(xmin+0.5*step);
  his1->Fill(xmin+1.5*step);
  his1->Fill(xmin+1.5*step);
  his1->Fill(xmin+2.5*step);
  his1->Fill(xmin+2.5*step);
  his1->Fill(xmin+2.5*step);

  TH1F* his2 = new TH1F("testMeanErr","testMeanErr",3,0.5,3.5);
  //  double xmin = his2->GetXaxis()->GetXmin();
  //  double step = his2->GetBinWidth(1);
  his2->Fill(xmin+0.5*step);
  his2->Fill(xmin+1.5*step);
  his2->Fill(xmin+1.5*step);
  his2->Fill(xmin+2.5*step);
  //  his2->Fill(xmin+2.5*step);
  //  his2->Fill(xmin+2.5*step);

  double mean1 = 0;
  double meanSq1 = 0;
  double nent1 = 0;
  double err1 = 0;
  for( int ii = 1; ii <= his1->GetXaxis()->GetNbins(); ii++ ) {
    std::cout << ii << " DATA= " << " " << his1->GetBinCenter(ii) << " "  << his1->GetBinContent(ii) << " +- " << his1->GetBinError(ii) << std::endl;
    double center = his1->GetBinCenter(ii);
    double binc = his1->GetBinContent(ii)+his2->GetBinContent(ii);
    mean1 += center*binc;
    meanSq1 += center*center*binc;
    nent1 += binc;
    std::cout << ii<<" center "<<center<<" nent1 "<<nent1<<" mean1 "<<mean1<<" meanSq1 "<< meanSq1 <<" err1 "<<err1<< std::endl; // GDEB    
  }
  std::cout <<"MEANF  "<< mean1/nent1 << " +-  " << sqrt(meanSq1-mean1*mean1/nent1)/nent1 << std::endl; //GDEB
  
  TH1F* hisN = new TH1F("testMeanErr","testMeanErr",3,0.5,3.5);
  *hisN = *his1+*his2;
  
  std::cout << " Mean= " << his1->GetMean() << " +- " << his1->GetMeanError() << "  " 
	    << his2->GetMean() << " +- " << his2->GetMeanError() << "  "
	    << hisN->GetMean() << " +- " << hisN->GetMeanError() << std::endl;
  std::cout << " RMS= " << his1->GetRMS() << " +- " << his1->GetRMSError() << "  "
	    << his2->GetRMS() << " +- " << his2->GetRMSError() << "  "
	    << hisN->GetRMS() << " +- " << hisN->GetRMSError() << std::endl;
  
  theCanvas->Update(); 
  hisN->Draw();
  theCanvas->Print("hisTestMeanErr.gif");
  
  file->cd();
  hisN->Write();
  
  file->Close();
  
}
