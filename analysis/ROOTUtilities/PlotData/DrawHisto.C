#ifndef DrawHisto_C
#define DrawHisto_C

#include "../GetWordsInLine.C"
#include "../GenUtils.C"
#include "../Verbosity.C"

#include "Definitions.C"

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
#include "TColor.h"
#include <math.h>
#include <map>
#include <vector>
#include <algorithm>
#include "TLine.h"

//------------------------------------------------------------------------
void SetHistoLimits(mdpdd* data, bool bInit )
{
  bool bOnlyXNonZero = true;
  bool bOnlyYNonZero = true;
  if( bInit ) {
    Definitions::xDataMin = DBL_MAX;
    Definitions::xDataMax = 0.;
    Definitions::yDataMin = DBL_MAX;
    Definitions::yDataMax = 0.;
  }
  //  verbose = 2;
  mdpdd::const_iterator ite;
  for( ite = data->begin(); ite != data->end(); ite++ ) {
    double valX = (*ite).first;
    double valY = (*ite).second.first;
    double errY = (*ite).second.second;
    if( verbose >= 4 ) std::cout << "SetHistoLimits: xdata MIN " << Definitions::xDataMin << " " << valX << std::endl; 
    if( verbose >= 4 ) std::cout << "SetHistoLimits: ydata MIN " << Definitions::yDataMin << " " << valY << std::endl;
    if( bOnlyYNonZero && valY != 0. ) {
      if( Definitions::bLogX && valX <= 0. ) {
	Definitions::bLogX = 0;
	theCanvas->SetLogx(0);
	std::cerr << "        issued by : SetHistoLimits " << std::endl;
	std::cerr << "  Asking for logarithmic X axis while values are <= 0 : " << valX << std::endl;
	std::cerr << "  *** This is just a warning message. *** " << std::endl;
	SetHistoLimits( data, bInit );
	return;
      }
      if( Definitions::bLogY && valY < 0. ) {
	Definitions::bLogY = 0;
	theCanvas->SetLogy(0);
	std::cerr << "        issued by : SetHistoLimits " << std::endl;
	std::cerr << "  Asking for logarithmic Y axis while values are < 0 : " << valX << std::endl;
	std::cerr << "  *** This is just a warning message. *** " << std::endl;
	SetHistoLimits( data, bInit );
	return;
      }
      if( !Definitions::bLogX || !bOnlyXNonZero || valX != 0. ) {
	Definitions::xDataMin = std::min( valX, Definitions::xDataMin );
	Definitions::xDataMax = std::max( valX, Definitions::xDataMax );
      }
      if( !Definitions::bLogY || valY != 0. ) {
	Definitions::yDataMin = std::min( valY-errY, Definitions::yDataMin );
	Definitions::yDataMax = std::max( valY+errY, Definitions::yDataMax );
      }
    }
    if( verbose >= 4 ) std::cout << "SetHistoLimits: xdata MIN " << Definitions::xDataMin << " " << valX << std::endl; 
    if( verbose >= 4 ) std::cout << "SetHistoLimits: ydata MIN " << Definitions::yDataMin << " " << valY << " +- " << errY << std::endl;

  }

  if( verbose >= 2 ) std::cout << "SetHistoLimits: XDATA MIN " << Definitions::xDataMin << " MAX " << Definitions::xDataMax << std::endl; 
  if( verbose >= 2 ) std::cout << "SetHistoLimits: YDATA MIN " << Definitions::yDataMin << " MAX " << Definitions::yDataMax << std::endl;

}

//------------------------------------------------------------------------
enum WHLType {WHL_None, WHL_Min, WHL_Max, WHL_MinMax};
//void WidenHistoLimits( double wideFactor = 0.05, WHLType whlType = WHL_MinMax)
void WidenHistoLimits( double wideFactor , WHLType whlTypeX = WHL_MinMax,  WHLType whlTypeY = WHL_MinMax)
{
  if( verbose >= 2 ) std::cout << " WIDENING XDATA MIN " << Definitions::xDataMin << " MAX " << Definitions::xDataMax << " Definitions::bLogX " << Definitions::bLogX << " whlTypeX " << whlTypeX << std::endl; 
  if( verbose >= 2 ) std::cout << " WIDENING YDATA MIN " << Definitions::yDataMin << " MAX " << Definitions::yDataMax << " Definitions::bLogY " << Definitions::bLogY << " whlTypeY " << whlTypeY << std::endl; 
  if( !Definitions::bLogX ) {
    double xStep = Definitions::xDataMax - Definitions::xDataMin;
    if( whlTypeX == WHL_MinMax || whlTypeX == WHL_Min) Definitions::xDataMin -= xStep * wideFactor;
    if( whlTypeX == WHL_MinMax || whlTypeX == WHL_Max) Definitions::xDataMax += xStep * wideFactor;
  } else {
    double xStep = Definitions::xDataMax / Definitions::xDataMin;
    if( whlTypeX == WHL_MinMax || whlTypeX == WHL_Min) Definitions::xDataMin /= std::pow(xStep, wideFactor);
    if( whlTypeX == WHL_MinMax || whlTypeX == WHL_Max) Definitions::xDataMax *= std::pow(xStep, wideFactor);
  }
  if( !Definitions::bLogY ) {
    double yStep = Definitions::yDataMax - Definitions::yDataMin;
    if( whlTypeY == WHL_MinMax || whlTypeY == WHL_Min) Definitions::yDataMin -= yStep * wideFactor;
    if( whlTypeY == WHL_MinMax || whlTypeY == WHL_Max) Definitions::yDataMax += yStep * wideFactor;
  } else {
    double yStep = Definitions::yDataMax / Definitions::yDataMin;
    if( whlTypeY == WHL_MinMax || whlTypeY == WHL_Min) Definitions::yDataMin /= std::pow(yStep, wideFactor);
      std::cout << "WIDE YMAX? " << whlTypeY << std::endl;
    if( whlTypeY == WHL_MinMax || whlTypeY == WHL_Max)  Definitions::yDataMax *= std::pow(yStep, wideFactor);
  }

  if( verbose >= 1 ) std::cout << " WIDENED XDATA MIN " << Definitions::xDataMin << " MAX " << Definitions::xDataMax << " Definitions::bLogX " << Definitions::bLogX << std::endl; 
  if( verbose >= 1 ) std::cout << " WIDENED YDATA MIN " << Definitions::yDataMin << " MAX " << Definitions::yDataMax << " bLogY " << Definitions::bLogY << std::endl;

}


//---------------------------------------------------------------------------
//--- Draw an emtpy histogram with minimum and maximum histogram values
TH1F* DrawEmptyHistogram( std::string histoName, double HXMin, double HXMax, double HYMin, double HYMax, TPad* pad )
{
 if( verbose >= -1 ) std::cout << " DrawEmptyHistogram " << histoName << " XMin " << HXMin << " XMax " << HXMax << " YMin " << HYMin << " YMax " << HYMax << std::endl;

  if( HXMin == 0. && HXMax == 0. ) {
    pad->SetLogy(0);
  } else {
    if( pad->GetLogy() && HXMin == 0. ) {
      HXMin = 1.E-9;
    }
    if( Definitions::bLogX) pad->SetLogx(1);
  }
  if( HYMin == 0. && HYMax == 0. ) {
    pad->SetLogy(0);
  } else {
    if( pad->GetLogy() && HYMin == 0. ) {
      HYMin = 1.E-9;
    }
    if( Definitions::bLogY) pad->SetLogy(1);
  }
  if( HYMin == HYMax ) {
    HYMin /= 10; // big enough so that it shows axis marks
    HYMax *= 10;
  }
  if( HXMin == HXMax ) {
    HXMin /= 10; // big enough so that it shows axis marks
    HXMax *= 10;
  }
  pad->cd();

  //Draw empty histogram
  /*  gStyle->SetPadRightMargin(Definitions::padRightMargin);
  gStyle->SetPadLeftMargin(Definitions::padLeftMargin);
  pad->cd();
  TPad* pad = new TPad("pad","pad",Definitions::padLeftMargin,0.0,1.-Definitions::padRightMargin,1.);
  pad->SetLeftMargin(Definitions::padLeftMargin); // does not work
  pad->SetLeftMargin(Definitions::padRightMargin);
  std::cout << " SetPadLeftMargin " << Definitions::padLeftMargin << std::endl; //GDEB
  pad->Draw();
  pad->cd(); */
  TH1F* his = new TH1F( histoName.c_str(), histoName.c_str(),100,HXMin,HXMax);
  his->SetTitle(histoName.c_str());
  his->SetMinimum(HYMin);
  //  his->SetMinimum(1.e-11);
  his->SetMaximum(HYMax);
  his->GetXaxis()->SetTitle(Definitions::xAxisTitle.c_str());
  his->GetYaxis()->SetTitle(Definitions::yAxisTitle.c_str());
  his->GetXaxis()->SetTitleOffset(Definitions::xAxisTitleOffset);
  his->GetYaxis()->SetTitleOffset(Definitions::yAxisTitleOffset);
  his->GetXaxis()->SetTitleSize(Definitions::xAxisTitleSize);
  his->GetYaxis()->SetTitleSize(Definitions::yAxisTitleSize);
  his->GetXaxis()->SetLabelSize(Definitions::xAxisLabelSize);
  his->GetYaxis()->SetLabelSize(Definitions::yAxisLabelSize);
  //  std::cout << " TITLE SIZE " << his->GetYaxis()->GetTitleSize() << " " << (Definitions::yAxisTitleSize) << std::endl; //GDEB
  //  std::cout << " LABEL OFFS " << his->GetXaxis()->GetLabelOffset() << std::endl; //GDEB
  his->GetXaxis()->SetLabelOffset(Definitions::xAxisLabelOffset);//-0.01);
  his->GetYaxis()->SetLabelOffset(Definitions::yAxisLabelOffset);
  //  std::cout << " LABEL OFFS " << his->GetXaxis()->GetLabelOffset() << std::endl; //GDEB
  /*  std::cout << " TICK " << pad->GetTickx() << std::endl; //GDEB
  pad->SetTickx(2.5);
  pad->SetTicky(22.5);
  std::cout << " TICK " << pad->GetTickx() << std::endl; //GDEB
  */
  his->Draw("");
  if( verbose >= 1 ) std::cout << "DRAW EMPTY HISTO " << histoName << " XMIN " << HXMin << " XMAX " << HXMax << " YMIN= " << his->GetMinimum() << " YMAX= " << his->GetMaximum() << " HYMin " << HYMin << " HYMax " << HYMax << std::endl;
  if( verbose >= 1 ) std::cout << "XYDRAW EMPTY HISTO " << histoName << " X " << his->GetXaxis()->GetTitle() << " Y= " << his->GetYaxis()->GetTitle() << " XSIZE " << his->GetXaxis()->GetTitleSize() << " YSIZE= " << his->GetYaxis()->GetTitleSize() << std::endl; //GDEB

  return his;
}

//-----------------------------------------------------------------------------
//--- Extract minimum and maximum histogram values and draw an empty histogram
TH1F* DrawEmptyHistogram( mdpdd* DataVal, std::string histoName, TPad* pad )
{
  SetHistoLimits( DataVal, true );

  return DrawEmptyHistogram( histoName, Definitions::xDataMin, Definitions::xDataMax, Definitions::yDataMin, Definitions::yDataMax, pad );
}  


//----------------------------------------------------------------------------
void DrawHisto( mdpdd* DataVal, EColor color, int marker, std::string histoName = "", bool bError = true)
{
  gStyle->SetOptStat(0);
  if( verbose >= -2 ) std::cout << "$$$$$$$$$$$ DrawHisto " << DataVal->size() << " name " << histoName << " MARKER " << marker << " COLOR " << color <<" N= " << DataVal->size() << std::endl;

  mdpdd::const_iterator iteE,iteEprev;
  bool bFirstPoint = true;
  double valTOT = 0.;
  for( iteE = DataVal->begin(); iteE != DataVal->end(); iteE++) {
    //    std::cout << "DrawHisto XDATA " << (*iteE).first << " " << Definitions::xDataMin << " " << Definitions::xDataMax << std::endl;
    //    if( (*iteE).first < Definitions::xDataMin || (*iteE).first > Definitions::xDataMax ) continue;   
    double xData = (*iteE).first;
    double val = (*iteE).second.first; 
    valTOT += val *log(188./168.);
    //    std::cout << xData << " VAL " << val/(1e10/1.6/log(188./168.)) << std::endl;
    if( xData < Definitions::xDataMin || xData > Definitions::xDataMax ) continue;
    //    if( bLogX ) xData = log10(xData);

    double error = 0.;
    if( bError ) error = (*iteE).second.second; 
    TMarker* mark = new TMarker(xData,val,marker);
    mark->SetMarkerColor(color);
    mark->SetMarkerSize(Definitions::markerSize);
    //    std::cout << histoName << " DrawHisto MARKER SIZE " << mark->GetMarkerSize() << " X= " << xData << " Y= " << val << std::endl;
    mark->Draw("");

    if( Definitions::lineJoinMarker ) {
      if( !bFirstPoint ) {
	iteEprev = iteE; iteEprev--;
	TLine* line = new TLine((*iteEprev).first,(*iteEprev).second.first,(*iteE).first,(*iteE).second.first);
	line->SetLineColor(color);
	line->SetLineWidth(theCanvas->GetWw()/500.);
	line->Draw("");
      }
      if( bError ) {
	TLine* line = new TLine(xData,val-error,xData,val+error);
	line->SetLineColor(color);
	line->SetLineWidth(theCanvas->GetWw()/500.);
	if( verbose >= 3 ) std::cout << " DrawHisto ERROR " << xData << " : " << val << " +- "<< error << " : " << val-error << " <> " << val+error << std::endl;
	line->Draw();
      }
    }
    bFirstPoint = false;
    if( verbose >= 3 ) std::cout << " DrawHisto " << xData << " " << val << " color " << color << std::endl;
  }

  if( verbose >= 2 ) std::cout << histoName << " TOTAL VALUE " << valTOT << std::endl;
  //  histoName = (itoa(int(color)) + "_" + histoName ).c_str();
  //  std::cout << "$$$$$$$$$$$ DrawHisto " << color << " " << itoa(int(color)) << " name " << histoName << std::endl;
  int icolor = int(color-1); if(icolor == 10 ) icolor = 9;
  //  double YHIGH = 200.;
  //  double xtext = Definitions::xDataMin+icolor%3*(Definitions::xDataMax-Definitions::xDataMin)/3.;
  //  double ytext = YHIGH/pow(8.,icolor/3);
  //-  theCanvas->cd();
  //  std::cout << " LINE NAME " << xtext << " " << ytext << " " << histoName << std::endl;
}  


//------------------------------------------------------------------
void DrawHistoRatio( mdpdd* DataVal1, mdpdd* DataVal2, std::string name1, std::string name2)
{
  double YRatio,YRatioAnt;

  std::string name = name1;
  if( name2 != "" ) name += "_" + name2;

  mdpdd::const_iterator iteE1,iteE2;
  mdpdd::const_iterator iteE1ant,iteE2ant;
  mdpdd::const_iterator iteE1ratio,iteE2ratio;
  std::string hminChar = ftoa(Definitions::xDataMin);
  std::string hmaxChar = ftoa(Definitions::xDataMax);

    /*char* hminChar = new char[5];
  sprintf(hminChar,"%e",Definitions::xDataMin);
  char* hmaxChar = new char[5];
  sprintf(hmaxChar,"%e",Definitions::xDataMax);
    */
  // Draw ratio 1 - 2
  bool Eequal=true;
  //--- Check that both vectors has same xData
  for( iteE1 = DataVal1->begin(),iteE2 = DataVal2->begin();iteE1 != DataVal1->end();iteE1++, iteE2++) {
    if( (*iteE1).first < Definitions::xDataMin || (*iteE1).first > Definitions::xDataMax ) continue;
    
    //	  std::cout << " bin xData" << "E1="<< (*iteE1).first << " E2= " << (*iteE2).first << " " << (*iteE1).first - (*iteE2).first << std::endl;
    if (fabs((*iteE1).first - (*iteE2).first) > (*iteE1).first*1.e-5 ){
      Eequal=false;
      if( verbose >= 0 ) std::cout << "different bin xData" << "E1= "<< (*iteE1).first << " E2= " << (*iteE2).first << " " << (*iteE1).first - (*iteE2).first << std::endl;
      break;
    }
  }
    
  float YRatioMIN = 9.E99;
  float YRatioMAX = 9.E-99;
  iteE2ant = DataVal2->begin();
  if ( Eequal){
    for( int iR = 0 ; iR < 2 ; iR++ ) {//first time in same sheet, second in separate sheet	
      TH1F* hisR = 0;
      if( iR == 0 ) {
	hisR = new TH1F( (name+"RATIO").c_str(),(name+"_RATIO").c_str(),100,Definitions::xDataMin,Definitions::xDataMax);
      }	 
      if( iR == 1 ) {
	hisR->SetMinimum(YRatioMIN/2);
	hisR->SetMaximum(YRatioMAX*2);
	hisR->Draw("");	
      }
      for( iteE1 = DataVal1->begin(), iteE2 = DataVal2->begin(); iteE1 != DataVal1->end();iteE1++, iteE2++) {
	if( iteE2 == DataVal2->end()) break;
	if( (*iteE1).first < Definitions::xDataMin || (*iteE1).first > Definitions::xDataMax ) continue;
	if( iteE2 != DataVal2->begin()  ) {
	  iteE2ant = iteE2; iteE2ant--;
	  iteE1ant = iteE1; iteE1ant--;
	  if((*iteE1).second.first == 0 ){
	    YRatio=1;
	  } else {
	    YRatio=(*iteE2).second.first / (*iteE1).second.first;
	  }
	  if((*iteE1ant).second.first==0 ){
	    YRatioAnt=1;
	  } else {
	    YRatioAnt=(*iteE2ant).second.first / (*iteE1ant).second.first;
	  }
	  
	  TLine* line = new TLine((*iteE2ant).first,YRatioAnt,(*iteE2).first,YRatio);
	  line->SetLineColor(kGreen);
	  line->Draw("");
	  if( iR == 0 ) {
	    if( YRatio < YRatioMIN ) YRatioMIN = YRatio;
	    if( YRatio > YRatioMAX ) YRatioMAX = YRatio;
	  }
	}
	//     TMarker* mark = new TMarker(log10(*iteE),*iteXS,22);
	//     mark->Draw();
      }
      
      if( iR == 0 ) theCanvas->Print( (name1+"."+name2+"."+hminChar+"-"+hmaxChar+".RATIO.gif").c_str()); 
      if( iR == 1 ) theCanvas->Print( (name1+"."+name2+"."+hminChar+"-"+hmaxChar+".RATIOONLY.gif").c_str()); 
    } 
  } else {
    for( int iR = 0 ; iR < 2 ; iR++ ) {//first time in same sheet, second in separate sheet
      TH1F* hisR = new TH1F( (name+"RATIO").c_str(),(name+"_RATIO").c_str(),100,Definitions::xDataMin,Definitions::xDataMax);
      if( iR == 1 ) {
	hisR->SetMinimum(YRatioMIN/2);
	hisR->SetMaximum(YRatioMAX*2);
	hisR->Draw("");	
      }
      bool bFirst = true;
      for( iteE1 = DataVal1->begin(); iteE1 != DataVal1->end();iteE1++) {
	if( (*iteE1).first < Definitions::xDataMin || (*iteE1).first > Definitions::xDataMax ) continue;
	// loop to find corresponding entry (same xData)
	//	    std::vector<std::pair<double,double> >::const_iterator iteE12found = DataVal2->end();
	for( iteE2 = iteE2ant; iteE2 != DataVal2->end();iteE2++) {
	  if( fabs( (*iteE1).first - (*iteE2).first )/(*iteE1).first < 1.E-8 ) {
	    //		iteE12found = iteE2;
	    break;
	  } 
	} 
	
	if( iteE2 != DataVal2->end() ) {
	  if( bFirst  ) {
	    bFirst = false;
	  } else {
	    
	    if((*iteE1).second.first==0 ){
	      YRatio=1;
	    } else {
	      YRatio = (*iteE2).second.first / (*iteE1).second.first;
	      if( iR == 0 && fabs(YRatio - 1.) > 0.01 && name2 == "G4" ) std::cout << "!!!!! WRONG RATIO in " << " " << name2 << "/" << name1 << " " << YRatio << " E2= " << (*iteE2).first << " E1= " << (*iteE1).first << " log " << log10( (*iteE1).first) << " XS2= " << (*iteE2).second.first << " XS1 " << (*iteE1).second.first << std::endl;
	      //		  if( iR == 0 && YRatio != 1. && name2 == "G4" ) std::cout << "!!!!! WRONG RATIO in " << histoName << " " << name2 << "/" << name1 << " " << YRatio << " E2= " << (*iteE2).first << " E1= " << (*iteE1).first << " log " << log10( (*iteE1).first) << " XS2= " << (*iteE2).second << " XS1 " << (*iteE1).second << std::endl;
	    }
	    if((*iteE1ant).second.first==0 ){
	      YRatioAnt=1;
	    } 
	    else {
	      YRatioAnt=(*iteE2ant).second.first / (*iteE1ant).second.first;
	    }
	    
	    TLine* line = new TLine(log10((*iteE2ant).first),YRatioAnt,log10((*iteE2).first),YRatio);
	    line->SetLineColor(kGreen);
	    line->Draw("");
	    if( iR == 0 ) {
	      if( YRatio < YRatioMIN ) YRatioMIN = YRatio;
	      if( YRatio > YRatioMAX ) YRatioMAX = YRatio;
	    }
	  }
	  iteE2ant = iteE2;
	  iteE1ant = iteE1;
	} 
	
	//     TMarker* mark = new TMarker(log10(*iteE),*iteXS,22);
	//     mark->Draw();
      }
      
      if( iR == 0 ) theCanvas->Print( (name+"."+hminChar+"-"+hmaxChar+".RATIO.gif").c_str()); 
      if( iR == 1 ) theCanvas->Print( (name+"."+hminChar+"-"+hmaxChar+".RATIOONLY.gif").c_str()); 
    } 
  }
}

void PrintGif( std::string hisname )
{
  theCanvas->Draw();
  theCanvas->Print( (hisname+".gif").c_str() );
}

#endif

