#if !defined(__CINT__) || defined(__MAKECINT__)

#include "./compare2.C"
#endif

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
void compareAll(const char* filename1, const char* filename2, bool bnorm = 0, bool blogy = 0 ) 
{
  
  theSampleTitle.push_back((char *)("HISTOS1"));
  theSampleTitle.push_back((char *)("HISTOS2"));
 
  InitComparison(filename1, filename2);
  theOptLogy = blogy;

  theGlobalTitle = (char *)("");
  
  TIter ListOfHistograms( theFiles[0]->GetListOfKeys() );
  TKey *HistoKey;

  TH1F *histo1 = 0;
  
  int nh = 0;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName()+std::string(";1");
    //ONLY HISTOGRAMS THAT CONTAIN 'NAME'     if( hisname.find("1E4") == std::string::npos ) continue;
    //ONLY HISTOGRAMS THAT DO NOT CONTAIN 'NAME'     if( hisname.find("NAME") != std::string::npos ) continue;
    TObject *obj = HistoKey->ReadObj();
    if( dynamic_cast<TH1F*>(obj) ) {
      histo1 = dynamic_cast<TH1F*>(obj);
      std::cout << " comparing " << histo1->GetName() << std::endl;
      compare2His(histo1->GetName(),histo1->GetName(),"N entries",histo1->GetName(), bnorm);
    } else {
      compare2HisProf(histo1->GetName(),histo1->GetName(),"N entries",histo1->GetName(), bnorm);
      std::cout << "NO 1D " << histo1->GetName() << std::endl; //GDEB 
    }
  }
  
}

