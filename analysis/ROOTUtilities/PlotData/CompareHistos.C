#ifndef CompareHistosXS_C
#define CompareHistosXS_C

#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMarker.h"
#include "TPad.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include "TLine.h"
#include "TText.h"
#include "TColor.h"
#include <math.h>
#include <map>
#include <vector>
#include <algorithm>
#include "TLine.h"

#include "ROOTUtilities/PlotData/DrawHisto.C"
#include "ROOTUtilities/Verbosity.C"
bool bPrintRatio;

//--- Compare a histogram in DataMap1 with name name1 with an histogram in DataMap2 with name name2, and prints them in a file whose name includes name1 and name2
void CompareHistosByNames(msmdpdd DataMap1, msmdpdd DataMap2, std::string name1, std::string name2, std::string fileName = "")
{
  if( verbose >= 2) std::cout << " XDATAMAX 0 " << xdataMax << std::endl;
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  if( verbose >= 1 ) std::cout << " @@@ CompareHistosByNames " << name1 << " vs " << name2 << std::endl;

  if( DataMap1.size() == 0 ) {
    std::cerr << "!!! CompareHistosByNames: No data in " << name1 << std::endl;
  }
  //  template<class TT> 
  msmdpdd::const_iterator iteM1, iteM2;
  if( verbose >= 1 ) {
    for( iteM1 = DataMap1.begin(); iteM1 != DataMap1.end(); iteM1++ ){
      if( verbose >= 1 ) std::cout << " @@@ CompareHistosByNames Map1 " << (*iteM1).first << " " << (*iteM1).second->size() << std::endl;
    }
    for( iteM2 = DataMap2.begin(); iteM2 != DataMap2.end(); iteM2++ ){
      if( verbose >= 1 ) std::cout << " @@@ CompareHistosByNames Map2 " << (*iteM2).first  << " " << (*iteM2).second->size() << std::endl;
    }
  }
  theCanvas->SetFillColor(0);
  theCanvas->Draw();
  if( bLogX ) theCanvas->SetLogx(1);
  if( bLogY ) theCanvas->SetLogy(1);
  

  iteM1 = DataMap1.find(name1);
  if( iteM1 == DataMap1.end() ) {
    std::cerr << " ERROR: " << name1 << " not found " << std::endl;
    for( iteM1 = DataMap1.begin(); iteM1 != DataMap1.end(); iteM1++ ){
      std::cerr << " DataMap1 " << (*iteM1).first << std::endl;
    }
    return;
  }

  mdpdd* DataVal1 = (*iteM1).second;

  if( verbose >= 2) std::cout << " XDATAMAX 1 " << xdataMax << std::endl;
  DrawEmptyHistogram( DataVal1, name1 );

  // Draw one histogram of set 1
  if( verbose >= 1 ) std::cout << "DRAWING HISTO 1 "  << name1 << " xdMin " << xdataMin << " xdMax " << xdataMax << std::endl;
  DrawHisto( DataVal1, kBlue, 21, name1);
  

  //---- Find data 2 to plot    ///		
  iteM2 = DataMap2.find(name2);
  if( iteM2 == DataMap2.end() ) {
    std::cerr << " ERROR: " << name2 << " not found " << std::endl;
    for( iteM2 = DataMap2.begin(); iteM2 != DataMap2.end(); iteM2++ ){
      std::cerr << " DataMap2 " << (*iteM2).first << std::endl;
    }
    return;
  }

  mdpdd* DataVal2 = (*iteM2).second;

  // Draw one histogram of set 2
  if( verbose >= 1 ) std::cout << "DRAWING HISTO 2 "  << name2 << " xdMin " << xdataMin << " xdMax " << xdataMax << std::endl;
  DrawHisto( DataVal2, kRed, 22, name2 );

  // print a gif file      
  // include energies in name
  char* xdMinChar = new char[5];
  sprintf(xdMinChar,"%f",xdataMin);
  char* xdMaxChar = new char[5];
  sprintf(xdMaxChar,"%f",xdataMax);
  //      std::cout << " XDMINCHAR " << xdMinChar << " = " << xdataMin << std::endl;
  theCanvas->Print( (name1+"."+name2+"."+fileName+"."+xdMinChar+"-"+xdMaxChar+".gif").c_str()); 
  
  if( bPrintRatio )  {
    DrawHistoRatio( DataVal1, DataVal2, name1, name2 );
  }

}

//--- Compare each histogram in DataMap1 with an histogram in DataMap2 with the same name, and prints them in a file whose name includes name1 and name2
void CompareHistos(msmdpdd DataMap1, msmdpdd DataMap2, const std::string prefixName, const std::string name1, const std::string name2, bool bDrawOnlyIfEqual = false )
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  if( verbose >= 1 ) std::cout << " @@@ CompareHistos " << name1 << " vs " << name2 << std::endl;

  if( DataMap1.size() == 0 ) {
    std::cerr << "!!! CompareHistos: No data in " << name1 << std::endl;
  }
  //  template<class TT> 
  msmdpdd::const_iterator iteM1, iteM2;

  theCanvas->SetFillColor(0);
  theCanvas->Draw();
  if( bLogX ) theCanvas->SetLogx(1);
  if( bLogY ) theCanvas->SetLogy(1);
  
  mdpdd* DataVal1;
  mdpdd* DataVal2;
  mdpdd::const_iterator iteE1,iteE2;

  for( iteM1 = DataMap1.begin(); iteM1 != DataMap1.end(); iteM1++ ){
    std::string channelName = (*iteM1).first;

    DataVal1 = (*iteM1).second;

    iteM2 = DataMap2.find(channelName);
    if( bDrawOnlyIfEqual && iteM2 == DataMap2.end() ) {
      if( verbose >= 1 ) std::cout << "CompareHistos Histo in Map1 and not in Map2 " << channelName << std::endl;
      continue;
    }
    //    if(   channelName.find("nCapture") != std::string::npos  ) continue;  
    //    if( channelName.find("hElas") != std::string::npos ) break;

    //----- Define histogram
    //--- Set minimum and maximum for histogram
    
    double XSMin = DBL_MAX;
    //(*(std::min_element(DataVal1->begin(),DataVal1->end()))).first;
    double XSMax= -DBL_MAX;
    for( iteE1 = DataVal1->begin(); iteE1 != DataVal1->end(); iteE1++) {
      if( (*iteE1).first < xdataMin || (*iteE1).first > xdataMax ) continue;
      double val = (*iteE1).second.first;
      if( val != 0. ) {
	if( XSMin > val ) XSMin = val;
	if( XSMax < val ) XSMax = val;
      }
      //      std::cout << " VAL " << val << " XSMin " << XSMin << " XSMax " << XSMax << std::endl;
    }

    if( XSMin == DBL_MAX ) { // no data in energy interval 
      continue;
    }

    if( XSMin == 0. && XSMax == 0. ) {
      theCanvas->SetLogy(0);
    } else {
      if( XSMin == 0. ) {
	XSMin = 1.E-9;
      }
      if( bLogY) theCanvas->SetLogy(1);
    }

    //Draw empty histogram
    TH1F* his = new TH1F( channelName.c_str(), channelName.c_str(),100,xdataMin,xdataMax);
    double xsdiff = pow(XSMax/XSMin,0.1);
    his->SetMinimum(XSMin/xsdiff);
    his->SetMaximum(XSMax*xsdiff);
    //    his->SetFillColor(0);
    his->Draw("");
    if( verbose >= 1 ) std::cout << "DRAW EMPTY HISTO " << channelName << " XMIN " << xdataMin << " XMAX " << xdataMax << " YMIN= " << his->GetMinimum() << " YMAX= " << his->GetMaximum() << " xsdiff " << xsdiff << " XSMin " << XSMin << " XSMax " << XSMax << std::endl;

    if(XSMin==0 && XSMax==0) continue;

    // Draw one histogram of set 1
    if( verbose >= 1) std::cout << " DRAWING HISTO 1 " << channelName << std::endl;
    DrawHisto( DataVal1, kBlue, 21, name1 );
    
    //---- Find data 2 to plot    ///		
    iteM2 = DataMap2.find(channelName);
    char* xdMinChar = new char[5];
    sprintf(xdMinChar,"%f",xdataMin);
    char* xdMaxChar = new char[5];
    sprintf(xdMaxChar,"%f",xdataMax);

    if( iteM2 != DataMap2.end() ) {
      DataVal2 = (*iteM2).second;
    // Draw one histogram of set 2
      if( verbose >= 1 ) std::cout << "DRAWING HISTO 2 "  << channelName << " xdMin " << xdataMin << " xdMax " << xdataMax << std::endl;
      DrawHisto( DataVal2, kRed, 22, name2 );
      // print a gif file      
  if( verbose >= 1) std::cout << " GIFNAME " << channelName+"."+name1+"."+name2+"."+xdMinChar+"-"+xdMaxChar+".gif" << " " << theCanvas << std::endl;
      theCanvas->Print( (prefixName+"."+name1+"."+name2+"."+channelName+"."+xdMinChar+"-"+xdMaxChar+".gif").c_str()); 
  if( verbose >= 1) std::cout << " GIF DONE " << std::endl;
      
      if( bPrintRatio )  {
	DrawHistoRatio( DataVal1, DataVal2, name1, name2 );
      } 
      
    } else { // print only first histogram 
      theCanvas->Print( (channelName+"."+name1+"."+xdMinChar+"-"+xdMaxChar+".gif").c_str()); 
      if( verbose >= 0 ) std::cout << " DATA IN " << name1 << " AND NOT IN " << name2 << " : " << channelName << std::endl;
    }	
  }

  /*  for( iteM1 = DataMap1.begin(); iteM1 != DataMap1.end(); iteM1++ ){
    iteM2 = DataMap2.find( channelName );
    if( iteM2 == DataMap2.end() ) {
    std::cout << " DATA IN " << name1 << " AND NOT IN " << name2 << " : " << channelName << std::endl;
    }
    }*/
  for( iteM2 = DataMap2.begin(); iteM2 != DataMap2.end(); iteM2++ ){
    iteM1 = DataMap1.find( (*iteM2).first );
    if( iteM1 == DataMap1.end() ) {
    if( verbose >= 0 ) std::cout << " DATA IN " << name2 << " AND NOT IN " << name1 << " : " << (*iteM2).first << std::endl;
    }
  }

}

#endif

