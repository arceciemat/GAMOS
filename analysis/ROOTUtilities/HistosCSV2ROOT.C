#ifndef HistoCSV2ROOT_C
#define HistoCSV2ROOT_C

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include <fstream>
#include "GetValue.C"
#include "GetWordsInLine.C"

void HistosCSV2ROOT( std::string fileName ) 
{
  std::cout << "HistoCSV2ROOT " << fileName << std::endl; //GDEB
  std::ifstream fin(fileName.c_str());
  if( !fin.is_open() ) {
    std::cerr << " HistoCSV2ROOT FILE NOT FOUND !!! " << fileName << std::endl;
    exit(1);
  }
  std::vector<std::string> wl;
  //  bool bReading = false;
  std::string depth;
  //  bool bFirst = true;
  std::vector<TH1F*> theHistos1;
  TH1F* his1 = 0;
  std::vector<TH2F*> theHistos2;
  TH2F* his2 = 0;
  int iReading = 1;
  double theMinX = 0;
  double theMinY = 0;
  double theStepX = 0;
  double theStepY = 0;
  double thePrecision = 0;
  for( ;; ) {
    if( !GetWordsInLine(fin, wl ) ) break;
    //    std::cout << " READING " << wl.size() << " " << wl[0] << std::endl; //GDEB
    if( wl[0] == "2D" ) {
      iReading = 2;
      int iw = 0;
      std::cout << " new TH2F( " << wl[1].c_str()<< " " << wl[1].c_str()<< " " << GetValue(wl[2])<< " " << GetValue(wl[3])<< " " << GetValue(wl[4])<< " " << GetValue(wl[5])<< " " << GetValue(wl[6])<< " " << GetValue(wl[7]) << std::endl; //GDEB
      his2 = new TH2F(wl[1].c_str(),wl[1].c_str(),GetValue(wl[2]),GetValue(wl[3]),GetValue(wl[4]),GetValue(wl[5]),GetValue(wl[6]),GetValue(wl[7]));
      theHistos2.push_back(his2);
      theMinX = GetValue(wl[3]);
      theStepX = (GetValue(wl[4])-theMinX)/GetValue(wl[2]);
      theMinY = GetValue(wl[6]);
      theStepY = (GetValue(wl[7])-theMinY)/GetValue(wl[5]);
      thePrecision = 1.e-3*std::min(theStepX,theStepY);
      std::cout << iw << " READ H " << wl[iw] << std::endl; //GDEB
      iw += 8;
      double nEntries = GetValue(wl[iw]);
      std::cout << iw << " READ nent " << wl[iw] << std::endl; //GDEB
      iw += 2;
      double meanX = GetValue(wl[iw]);
      double meanErrX = -DBL_MAX;
      if( wl[iw+1] == "+-" ) {
	meanErrX = GetValue(wl[iw]);
	iw += 2;
      }	
      iw += 2;
      double meanY = GetValue(wl[iw]);
      double meanErrY = -DBL_MAX;
      if( wl[iw+1] == "+-" ) {
	meanErrY = GetValue(wl[iw]);
	iw += 2;
      }
      std::cout << iw << " READ meanErrY " << wl[iw] << std::endl; //GDEB
      iw += 2;
      double rmsX = GetValue(wl[iw]);
      iw += 2;
      double rmsErrX = -DBL_MAX;
      if( wl[iw+1] == "+-" ) {
	rmsErrX = GetValue(wl[iw]);
	iw += 2;
      }	
      iw += 2;
      double rmsY = GetValue(wl[18]);
      iw += 2;
      double rmsErrY = -DBL_MAX;
      if( wl[iw+1] == "+-" ) {
	rmsErrY = GetValue(wl[iw]);
	iw += 2;
      }	
      iw += 2;
      std::cout << iw << " READ rmsErrY " << wl[iw] << std::endl; //GDEB
      double uflowX = GetValue(wl[iw]);
      iw += 2;
      double oflowX = GetValue(wl[iw]);
      iw += 2;
      double uflowY = GetValue(wl[iw]);
      iw += 2;
      double oflowY = GetValue(wl[iw]);
    } else {
      if( iReading == 2 ) {
	int ix = (GetValue(wl[0])-theMinX+thePrecision)/theStepX+1;
	int iy = (GetValue(wl[1])-theMinY+thePrecision)/theStepY+1;
	his2->SetBinContent(ix,iy,GetValue(wl[2]),GetValue(wl[3]));
	//	std::cout << " FILL HISTO " << ix << " " << iy << " " << GetValue(wl[0]) << " " << GetValue(wl[1]) << " " << GetValue(wl[2]) << " " << GetValue(wl[3]) << std::endl;
      }
    }
  }

  std::string fileNameROOT = fileName.substr(0,fileName.length()-3)+"root";
  TFile* file = new TFile(fileNameROOT.c_str(),"RECREATE");
  file->cd();
  for( size_t ii = 0; ii < theHistos2.size(); ii++ ) {
    theHistos2[ii]->Write();
  }
  file->Close();

}

#endif

