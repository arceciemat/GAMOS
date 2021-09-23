#include <cfloat>
#include "Verbosity.C"
#include "PlotData/Definitions.C"
#include "GetWordsInLine.C"
#include "GetValue.C"
#include "PlotData/DrawHisto.C"
#include "TSystem.h"
#include <fstream>
#include <vector>
#include <set>
#include <map>

void ReadMetaFile( std::string fMetaName );
std::set<int> theCSkip;
std::map<int, double> theHXMin;
std::map<int, double> theHXMax;
std::map<int, int> theHXNBins;

void columnToPlot( std::string fName, std::string fMetaName = "" )
{
  TCanvas* theCanvas = new TCanvas;
  if( fMetaName != "" ) ReadMetaFile( fMetaName );

  verbose = 1;
  TFile* file = new TFile("column.root","RECREATE");

  std::vector<std::string> wl;
  std::ifstream fin(fName.c_str());
  if( fin.fail() ) {
    std::cout << " FILE DOES NOT EXIST: " << fName << std::endl;
    exit(1);
  }
  std::map< int, std::vector<double>* > theData;
  for(size_t ii = 0;; ii++) {
    if( !GetWordsInLine(fin,wl) ) break;
    if( ii == 0 ) {
      for( size_t iw = 0; iw < wl.size(); iw++ ) {
	if(verbose >= 3) std::cout << iw << " WORD " << wl[iw] << std::endl; //GDEB
	if( theCSkip.find(iw) != theCSkip.end() ) continue;
	std::vector<double>* data1 = new  std::vector<double>;
	theData[iw] = data1;
	if(verbose >= 2) std::cout << iw << "CREATE DATA1 " << data1 << " " << theData.size() << std::endl;
      }
    }

    for( size_t iw = 0; iw < wl.size(); iw++ ) {
      if( theCSkip.find(iw) != theCSkip.end() ) continue;
      std::vector<double>* data1 = theData[iw];
      if(verbose >= 2) std::cout << iw << " DATA1 " << data1 << " " << theData.size() << std::endl;
      data1->push_back(GetValue(wl[iw]));
    }
  }

  if(verbose >= 2) std::cout << " DATA READ " << std::endl;
  std::map< int, std::vector<double>* >::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite ++ ) {
    int icol = (*ite).first;
    std::vector<double>* data1 = (*ite).second;
    /*    double hXMin = DBL_MAX;
    double hXMax = -DBL_MAX;
    for( size_i2 = 0; i2 < data1->size(); i2++ ) {
      if( 
    */
    std::string hName = "column_"+itoa(icol);
    //t    TH1F* his = new TH1F(hName.c_str(),hName.c_str(),theHXNBins[icol],theHXMin[icol],theHXMax[icol]);
    TH1F* his = new TH1F(hName.c_str(),hName.c_str(),100,theHXMin[icol],theHXMax[icol]);
    //    theHistos[icol] = his;
    for( size_t id = 0; id < data1->size(); id++ ) {
      his->Fill((*data1)[id]);
      if(verbose >= 2) std::cout << " FILL " << (*data1)[id] << std::endl;
    }

    theCanvas->Update();
    his->Draw();
    theCanvas->Print(std::string("his."+hName+".gif").c_str());
    std::ofstream fout("his."+hName+".txt");
    fout << " MEAN= " << his->GetMean() << " " << his->GetName() << std::endl;
    fout << " RMS= " << his->GetRMS() << " " << his->GetName() << std::endl;
    fout << " NENTRIES= " << his->GetEntries() << " " << his->GetName() << std::endl;
     
    file->cd();
    his->Write();
  }

  file->Close();
  
}


void ReadMetaFile( std::string fMetaName )
{
  std::vector<std::string> wl;
  std::ifstream fin(fMetaName.c_str());
  if( fin.fail() ) {
    std::cout << " FILE DOES NOT EXIST: " << fMetaName << std::endl;
    exit(1);
  }
  for(size_t ii = 0;; ii++) {
    if( !GetWordsInLine(fin,wl) ) break;  
    if( wl[0] == ":CSKIP" ) {
      if( wl.size() != 2 ) {
	cout << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have two words, while it has " << wl.size() << endl;
	exit(1);
      }
      theCSkip.insert(GetValInt(wl[1]));
    } else if( wl[0] == ":HXMIN" ) {
      if( wl.size() != 3 ) {
	cout << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have three words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXMin[GetValInt(wl[1])] = GetValue(wl[2]);
    } else if( wl[0] == ":HXMAX" ) {
      if( wl.size() != 3 ) {
	cout << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have three words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXMax[GetValInt(wl[1])] = GetValue(wl[2]);
    } else if( wl[0] == ":HXNBINS" ) {
      if( wl.size() != 3 ) {
	cout << "!!! ERROR:  ReadMetaFile line " << wl[0] << " must have three words, while it has " << wl.size() << endl;
	exit(1);
      }
      theHXNBins[GetValInt(wl[1])] = GetValInt(wl[2]);
    } else {
      	cout << "!!! ERROR:  ReadMetaFile unexpected line, starting with " << wl[0] << endl;
	exit(1);
    }
  }

  if(verbose >= 3) std::cout << " EXIT ReadMetaFile " << std::endl;
}
    
