#include <cfloat>
#include "ROOTUtilities/Verbosity.C"
#include "ROOTUtilities/PlotData/Definitions.C"
#include "ROOTUtilities/GetWordsInLine.C"
#include "ROOTUtilities/GetValue.C"
#include "ROOTUtilities/PlotData/DrawHisto.C"
#include "TSystem.h"
#include <fstream>
#include <vector>
#include <set>
#include <map>

int SHIFT;
void ReadMetaFile( std::string fMetaName );
std::map<int, double> theHXMin;
std::map<int, double> theHXMax;
std::map<int, int> theHXNBins;
std::map<int, double> theHYMin;
std::map<int, double> theHYMax;
std::map<int, int> theHYNBins;
std::set<int> thePlots;

void columns2ToPlot( std::string fName, std::string fMetaName = "" )
{
  SHIFT = 1000;
  TCanvas* theCanvas = new TCanvas;
  if( fMetaName != "" ) ReadMetaFile( fMetaName );

  verbose = 10;
  TFile* file = new TFile("column.root","RECREATE");

  std::vector<std::string> wl;
  std::ifstream fin(fName.c_str());
  std::map< int, std::vector<double>* > theData;
  for(size_t ii = 0;; ii++) {
    if( !GetWordsInLine(fin,wl) ) break;
    if( ii == 0 ) {
      for( size_t iw = 0; iw < wl.size(); iw++ ) {
	std::vector<double>* data1 = new  std::vector<double>;
	theData[iw] = data1;
	//	std::cout << iw << "CREATE DATA1 " << data1 << " " << theData.size() << std::endl;
      }
    }

    for( size_t iw = 0; iw < wl.size(); iw++ ) {
      std::vector<double>* data1 = theData[iw];
      //      std::cout << iw << " DATA1 " << data1 << " " << theData.size() << std::endl;
      data1->push_back(GetValue(wl[iw]));
    }
  }    

  std::cout << " DATA READ " << theData.size() << " PLOTS " << thePlots.size() << std::endl;
  std::set<int>::const_iterator ite;
  for( ite = thePlots.begin(); ite != thePlots.end(); ite++ ) {
    std::map< int, std::vector<double>* >::const_iterator ited = theData.find((*ite)/SHIFT);
    int icol1 = (*ited).first;
    std::vector<double>* data1 = (*ited).second;
    ited = theData.find((*ite)%SHIFT);
    int icol2 = (*ited).first;
    std::vector<double>* data2 = (*ited).second;
    
    /*    double hXMin = DBL_MAX;
    double hXMax = -DBL_MAX;
    for( size_i2 = 0; i2 < data1->size(); i2++ ) {
      if( 
    */
    std::string hName = "column_"+itoa(icol1)+"."+itoa(icol2);
    TH1F* his = new TH1F(hName.c_str(),hName.c_str(),theHXNBins[*ite],theHXMin[*ite],theHXMax[*ite]);
    std::cout << *ite << " HISTO DEFINE " << theHXNBins[*ite] << " " << theHXMin[*ite] << " " << theHXMax[*ite] << std::endl;
    his->SetMinimum(theHYMin[*ite]);
    his->SetMaximum(theHYMax[*ite]);
    std::cout << *ite << " HISTO DEFINE Y " << theHYMin[*ite] << " " << theHYMax[*ite] << std::endl;
    //    theHistos[icol] = his;
    for( size_t id = 0; id < data1->size(); id++ ) {
      his->Fill((*data1)[id],(*data2)[id]);
      //   std::cout << " FILL " << (*data1)[id] << " " << (*data2)[id] << std::endl;
    }

    theCanvas->Update(); 
    his->Draw();
    theCanvas->Print(std::string("his."+hName+".gif").c_str());

    file->cd();
    his->Write();
  }

  file->Close();
  
}


void ReadMetaFile( std::string fMetaName )
{
  std::vector<std::string> wl;
  std::ifstream fin(fMetaName.c_str());
  for(size_t ii = 0;; ii++) {
    if( !GetWordsInLine(fin,wl) ) break;  
    if( wl[0] == ":CPLOT" ) {
      if( wl.size() != 3 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have three words, while it has " << wl.size() << endl;
	exit(1);
      }
      thePlots.insert(GetValInt(wl[1])*SHIFT+GetValInt(wl[2]));
    } else if( wl[0] == ":HXMIN" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXMin[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else if( wl[0] == ":HXMAX" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXMax[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else if( wl[0] == ":HXNBINS" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXNBins[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else if( wl[0] == ":HYMIN" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHYMin[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else if( wl[0] == ":HYMAX" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHYMax[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else if( wl[0] == ":HYNBINS" ) {
      if( wl.size() != 4 ) {
	cerr << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have four words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHYNBins[GetValInt(wl[1])*SHIFT+GetValInt(wl[2])] = GetValue(wl[3]);
    } else {
      	cerr << "!!! ERROR:  ReadMetaFile unexpected line, starting with " << wl[0] << endl;
	exit(1);
    }
  }

  std::cout << " EXIT ReadMetaFile " << std::endl;
}
    
