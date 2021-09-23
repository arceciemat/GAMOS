#ifndef PlotUtils_C
#define PlotUtils_C
#include "Definitions.C"
#include "TText.h"
#include "TPaveText.h"
#include "TMarker.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TColor.h"
#include "../Verbosity.C"

#include <sstream>

//--------------------------------------------------------------------------
EColor GetColor( int icol )
{
  // kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
  // kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
  // kSpring = 820, kTeal   = 840, kAzure   = 860,  kViolet = 880,  kPink   = 900
  // KGreen+4     , kRed-2
  int icol1 = icol/14;
  int icol2 = icol%14;

  switch (icol2)  {
    case 0:
      return EColor(kBlack+icol1);
    case 1:
      return EColor(kRed);
    case 2:
      return EColor(kGreen);
    case 3:
      return EColor(kBlue);
    case 6:
      return EColor(kMagenta+icol1);
    case 4:
      return EColor(kYellow+icol1+1);
    case 5:
      return EColor(kCyan+icol1);
    case 7:
      return EColor(kViolet+2);
    case 8:
      return EColor(kOrange+icol1+1);
    case 9:
      return EColor(kOrange+3);
    case 11:
      return EColor(kPink+icol1+1);
    case 10:
      //      return EColor(kSpring+icol1+1);
      return EColor(kTeal-7);
    case 12:
      return EColor(kAzure+icol1+1); 
    case 13:
      return EColor(kGray+icol1);
    }
  
  return EColor(kBlack);
  
}

//--------------------------------------------------------------------------
int GetStyle( int isty ) //https://root.cern.ch/doc/master/classTAttMarker.html#ATTMARKER2
{

  int isty1 = isty%20;
  switch (isty1)  {
  case 0:
    return 20;
  case 1:
    return 24;
  case 2:
    return 21;
  case 3:
    return 25;
  case 4:
    return 22;
  case 5:
    return 26;
  case 6:
    return 23;
  case 7:
    return 27;
  case 8:
    return 29;
  case 9:
    return 30;
  case 10:
    return 33;
  case 11:
    return 34;
  case 12:
    return 39;
  case 13:
    return 41;
  case 14:
    return 43;
  case 15:
    return 44;
  case 16:
    return 47;
  case 17:
    return 46;
  case 18:
    return 38;
  case 19:
    return 48;
  case 20:
    return 40;
  }

  return 20;
  
}

//--------------------------------------------------------------------------
void DrawText( std::string tt, double xstart, double ystart, double textSize = 0.05 ) 
{
  std::string td = ""; 
  for( int ii = 0; ii < 10; ii++ ) {
    for( int ii2 = 0; ii2 < 10; ii2++ ) {
      td += tt;
    }
    TText* text = new TText(xstart,ystart*pow(1.E-4,ii),td.c_str());
    text->SetTextSize(textSize);
    text->Draw();
  }
}

//--------------------------------------------------------------------------
double GetXPos( double xPoints, double hxMin, double hxWidth )
{
  double xPos; 
  if( !Definitions::bLogX ) {
    xPos = hxMin + hxWidth * xPoints/Definitions::TOTAL_WIDTH;
  } else {
    xPos = hxMin * std::pow(hxWidth, xPoints/Definitions::TOTAL_WIDTH);
  }
if( verbose >= 2 ) std::cout << " GETXPOS " << xPos << " = hxMin " << hxMin << " + " << " hxWidth " << hxWidth << " * xPoints " << xPoints << " / " << Definitions::TOTAL_WIDTH << std::endl; //GDEB
  return xPos;
}

//--------------------------------------------------------------------------
double GetYPos( double yPoints, double hyMin, double hyWidth )
{
  double yPos; 
  if( !Definitions::bLogY ) {
    yPos = hyMin + hyWidth * yPoints/Definitions::TOTAL_HEIGTH;
  } else {
    yPos = hyMin * std::pow(hyWidth, yPoints/Definitions::TOTAL_HEIGTH);
  }
  //  std::cout << " GETYPOS " << yPos << " " << yPoints << " " << hyMin << " " << hyWidth << std::endl;
  return yPos;
}

double hxMin;
double hxMax;
double hyMin;
double hyMax;
double hxWidth;
double hyWidth;

//--------------------------------------------------------------------------
void CalculateHistoDims(TH1* his)
{
  hxMin = his->GetXaxis()->GetXmin();
  hxMax = his->GetXaxis()->GetXmax();
  hyMin = his->GetMinimum();
  hyMax = his->GetMaximum();
  if( !Definitions::bLogX ) {
    hxWidth = hxMax - hxMin;
  } else {
    hxWidth = hxMax / hxMin;
  }
  if( verbose >= 2 ) std::cout << " CalculateHistoDims " << Definitions::bLogX << " HXWIDTH " << hxWidth  << " min " << hxMin << " max " << hxMax << std::endl;
  if( !Definitions::bLogY ) {
    hyWidth = hyMax-hyMin;
  } else {
    hyWidth = hyMax/hyMin;
  }
  if( verbose >= 2 ) std::cout << " CalculateHistoDims " << Definitions::bLogY << " HYWIDTH " << hyWidth  << " min " << hyMin << " max " << hyMax << std::endl;

}
 
//--------------------------------------------------------------------------
void DrawGlobalTitle( TH1F* his, std::string title, EColor color = kBlack )
{
  return;
  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;
 //  std::cout << " hx " << hxMin << " " << hxMax << std::endl;
  //--- xPos at center, yPos 0.05 above frame
  double hyUp = 0.05;
  if( !Definitions::bLogY ) {
    yPos = hyMax + hyWidth*hyUp;
  } else {
    yPos = hyMax * std::pow(hyWidth,hyUp);
  }

  TText* textTitle = new TText(xPos, yPos, title.c_str());
  textTitle->SetTextColor(color);
  UInt_t tx = 1;
  UInt_t ty = 1;
  textTitle->GetTextExtent(tx,ty,title.c_str());
  //  std::cout << "TEXT SIZE " << tx << " " << ty << std::endl;
  if( tx > Definitions::TOTAL_WIDTH ) {
    textTitle->SetTextSize(1.1*0.05*Definitions::TOTAL_WIDTH/tx);
    textTitle->GetTextExtent(tx,ty,title.c_str());
    //    std::cout << "TEXT SIZE RESCALED " << tx << " " << ty << std::endl;
  }
  xPos = GetXPos( Definitions::TOTAL_WIDTH/2-tx/2, hxMin, hxWidth );

  textTitle->SetX(xPos);
  textTitle->Draw();

}

//--------------------------------------------------------------------------
void DrawXTitle( TH1F* his, std::string title, EColor color = kBlack )
{
  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;
  //--- xPos at right, yPos 0.05 below frame
  double hyDisp = 0.1;
  yPos = GetYPos( 0 - hyDisp * Definitions::TOTAL_HEIGTH, hyMin , hyWidth );
  /*  if( !Definitions::bLogY ) {
    yPos = hyMin - hyWidth*hyDisp;
  } else {
    yPos = hyMin / std::pow(hyWidth,hyDisp);
    } */
  //  std::cout << " hx " << hxMin << " " << hxMax << std::endl;

  TText* textTitle = new TText(xPos, yPos, title.c_str());
  textTitle->SetTextColor(color);
  textTitle->SetTextSize(0.03);
  UInt_t tx = 1;
  UInt_t ty = 1;
  textTitle->GetTextExtent(tx,ty,title.c_str());
  // displace text in X so that it end at left
  xPos = GetXPos( Definitions::TOTAL_WIDTH-tx, hxMin, hxWidth );
  textTitle->SetX(xPos);
  textTitle->Draw();

}

//--------------------------------------------------------------------------
void DrawYTitle( TH1F* his, std::string title, EColor color = kBlack )
{

  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;
  //--- xPos 0.07 at left, yPos at top
  double hxDisp = 0.07;
  if( !Definitions::bLogX ) {
    xPos = hxMin - hxWidth*hxDisp;
  } else {
    xPos = hxMin / std::pow(hxWidth,hxDisp);
  }
  //  std::cout << " hx " << hxMin << " " << hxMax << std::endl;

  TText* textTitle = new TText(xPos, yPos, title.c_str());
  textTitle->SetTextColor(color);
  textTitle->SetTextSize(0.03);
  textTitle->SetTextAngle(90.);
  UInt_t tx = 1;
  UInt_t ty = 1;
  textTitle->GetTextExtent(tx,ty,title.c_str());
  //--- displace text in Y so that it end at top
  //  std::cout << "TEXT SIZE " << tx << " " << ty << std::endl;
  yPos = GetYPos( Definitions::TOTAL_HEIGTH-tx, hyMin, hyWidth );
  textTitle->SetY(yPos);
  textTitle->Draw();

}

//--------------------------------------------------------------------------
double GetTextXExtent( std::string title, double size )
{

  TText* textTitle = new TText(0,0, title.c_str());
  textTitle->SetTextSize(size);
  UInt_t tx = 1;
  UInt_t ty = 1;
  textTitle->GetTextExtent(tx,ty,title.c_str());
  delete textTitle;
  return tx;

}

//--------------------------------------------------------------------------
double GetTextYExtent( std::string title, double size )
{

  TText* textTitle = new TText(0,0, title.c_str());
  textTitle->SetTextSize(size);
  UInt_t tx = 1;
  UInt_t ty = 1;
  textTitle->GetTextExtent(tx,ty,title.c_str());
  delete textTitle;
  return ty;

}

//--------------------------------------------------------------------------
void DrawWordInPave( TH1F* his, std::string title, double& xInit, double& yInit, EColor color = kBlack, double size = 0.03, bool bCheckProtrude = true )
{
  //  verbose = 1;
  //  return;
  if( verbose >= 2 ) std::cout << " DRAWWORDINPAVE " << his->GetName() << " : " << title << " Xinit=" << xInit << " Yinit=" << yInit << std::endl;

  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;

  UInt_t tx = 0;
  UInt_t ty = 0;
  TText* textTitle = 0;
  TPaveText* ptextTitle = 0;
  if( title != "" ) {
    textTitle = new TText(xPos, yPos, title.c_str());
    textTitle->SetTextSize(size);
    tx = 1;
    ty = 1;
    textTitle->GetTextExtent(tx,ty,title.c_str());
    ptextTitle = new TPaveText(xPos, yPos, xPos+tx, yPos+ty, "NB");
    ptextTitle->AddText(title.c_str());
    ptextTitle->SetTextColor(color);
    ptextTitle->SetTextSize(size);
  }

  if( bCheckProtrude && xInit + tx > Definitions::TOTAL_WIDTH ) {
    if( verbose >= 1 ) std::cout << " DrawWordInPave: PROTRUDE " << title << " " << xInit + tx << " > " << Definitions::TOTAL_WIDTH << " : " << xInit << " + " << " + " << tx  << std::endl; //GDEB
    xInit = 30;
    yInit -= 15;
  }
  if( title != "" ) {
    xPos = GetXPos( xInit, hxMin, hxWidth );
    yPos = GetYPos( yInit, hyMin, hyWidth );
    ptextTitle->SetX1(xPos);
    ptextTitle->SetY1(yPos);
    double xPos2 = GetXPos( xInit+tx, hxMin, hxWidth );
    double yPos2 = GetYPos( yInit+ty/2.+ty, hyMin, hyWidth );
    ptextTitle->SetX2(xPos2);
    ptextTitle->SetY2(yPos2);
    ptextTitle->SetFillStyle(0);
    ptextTitle->SetLineColor(kWhite);
    ptextTitle->Draw();
  }
  //--- reset xInit to end of word + 10
  if( verbose >= 2 ) {
    std::cout << " DRAWTEXT " << his->GetName() << " X=" << xPos << " Y=" << yPos << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " Xinit=" << xInit << " Yinit=" << yInit << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " tx=" << tx << " ty=" << ty << std::endl;
  }
  xInit += tx + 10;
}

//--------------------------------------------------------------------------
void DrawWordAndMarker( TH1F* his, std::string title, double& xInit, double& yInit, EColor color = kBlack, int iMarker = 20, double size = 0.03, bool bCheckProtrude = true )
{
  if( title == "" ) return;
  //  verbose = 1;
  //  return;
  if( verbose >= 2 ) std::cout << " DRAWWORDANDMARKER " << his->GetName() << " : " << title << " Xinit=" << xInit << " Yinit=" << yInit << " Color= " << color <<std::endl;

  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;

  UInt_t tx = 0;
  UInt_t ty = 0;
  TText* textTitle = 0;
  if( title != "" ) {
    textTitle = new TText(xPos, yPos, title.c_str());
    //    std::cout << " DrawWordAndMarker TText at " << xPos << " , " << yPos << std::endl; //GDEB
    textTitle->SetTextSize(size);
    tx = 1;
    ty = 1;
    textTitle->GetTextExtent(tx,ty,title.c_str());
  }

  //--- marker
  int markerWidth = size*50*2; 
  //--- Check if it protrudes
  //t  if( xInit + markerWidth + tx > Definitions::TOTAL_WIDTH ) {
  if( bCheckProtrude && xInit + markerWidth + tx > Definitions::TOTAL_WIDTH ) {
    if( verbose >= 1 ) std::cout << " DrawWordAndMarker: PROTRUDE " << title << " " << xInit + markerWidth + tx << " > " << Definitions::TOTAL_WIDTH << " : " << xInit << " + " << markerWidth << " + " << tx  << std::endl; //GDEB
    xInit = 30;
    yInit -= 15;
  }
  
  if( iMarker != 0 ) {
    xPos = GetXPos( xInit, hxMin, hxWidth );
    //  yPos = GetYPos( yInit+ty*1., hyMin, hyWidth );
    yPos = GetYPos( yInit+ty*Definitions::markerTextDispY, hyMin, hyWidth ); //FINAL
    //  std::cout << " DrawWordAndMarker TMarker at " << xPos << " , " << yPos << " " << yInit << " + " << ty << " * " << Definitions::markerTextDispY << " , " << hyMin << " " << hyWidth << " max " << hyMax << std::endl; //GDEB
    TMarker* marker = new TMarker(xPos, yPos, iMarker);
    marker->SetMarkerColor(color);
    marker->SetMarkerSize(Definitions::markerSize);
    marker->Draw(); 
  }
  
  if( title != "" ) {
    //    xInit += markerWidth*7;  
    //   xInit += markerWidth*10;
    xInit += markerWidth*Definitions::markerTextDispX; // FINAL
    xPos = GetXPos( xInit, hxMin, hxWidth );
    yPos = GetYPos( yInit, hyMin, hyWidth );
    textTitle->SetX(xPos);
    textTitle->SetY(yPos);
    textTitle->SetTextColor(color);
    textTitle->Draw();
  }
  //--- reset xInit to end of word + 10
  if( verbose >= 2 ) {
    std::cout << " DRAWTEXT " << his->GetName() << " X=" << xPos << " Y=" << yPos << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " Xinit=" << xInit << " Yinit=" << yInit << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " tx=" << tx << " ty=" << ty << std::endl;
  }

  xInit += tx + 10;
}

//--------------------------------------------------------------------------
void DrawWordAndMarker( TProfile* his, std::string title, double& xInit, double& yInit, EColor color = kBlack, int iMarker = 20, double size = 0.03, bool bCheckProtrude = true )
{
  //  verbose = 1;
  //  return;
 if( verbose >= 2 ) std::cout << " DRAWWORDANDMARKER " << his->GetName() << " : " << title << " Xinit=" << xInit << " Yinit=" << yInit << " Color= " << color <<std::endl;

  CalculateHistoDims(his);
  double xPos = 0.;
  double yPos = 0.;

  UInt_t tx = 0;
  UInt_t ty = 0;
  TText* textTitle = 0;
  if( title != "" ) {
    textTitle = new TText(xPos, yPos, title.c_str());
    textTitle->SetTextSize(size);
    tx = 1;
    ty = 1;
    textTitle->GetTextExtent(tx,ty,title.c_str());
  }

  //--- marker 
  int markerWidth = size*50*2; 
  //--- Check if it protrudes
  //t  if( xInit + markerWidth + tx > Definitions::TOTAL_WIDTH ) {
  if( bCheckProtrude && xInit + markerWidth + tx > Definitions::TOTAL_WIDTH ) {
    if( verbose >= 1 ) std::cout << " DrawWordAndMarker: PROTRUDE " << title << " " << xInit + markerWidth + tx << " > " << Definitions::TOTAL_WIDTH << " : " << xInit << " + " << markerWidth << " + " << tx  << std::endl; //GDEB
    xInit = 30;
    yInit -= size*1000;;
  }

  xPos = GetXPos( xInit, hxMin, hxWidth );
  //  yPos = GetYPos( yInit+ty*1., hyMin, hyWidth );
  yPos = GetYPos( yInit+ty*Definitions::markerTextDispY, hyMin, hyWidth ); //FINAL
  TMarker* marker = new TMarker(xPos, yPos, iMarker);
  marker->SetMarkerColor(color);
  marker->SetMarkerSize(Definitions::markerSize);
  marker->Draw();

  if( title != "" ) {
    //    xInit += markerWidth*7;  
    //   xInit += markerWidth*10;
    xInit += markerWidth*Definitions::markerTextDispX; // FINAL
    xPos = GetXPos( xInit, hxMin, hxWidth );
    yPos = GetYPos( yInit, hyMin, hyWidth );
    textTitle->SetX(xPos);
    textTitle->SetY(yPos);
    textTitle->SetTextColor(color);
    textTitle->Draw();
  }
  //--- reset xInit to end of word + 10
  if( verbose >= 2 ) {
    std::cout << " DRAWTEXT " << his->GetName() << " X=" << xPos << " Y=" << yPos << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " Xinit=" << xInit << " Yinit=" << yInit << std::endl;
    std::cout << " DRAWTEXT " << his->GetName() << " tx=" << tx << " ty=" << ty << std::endl;
  }

  xInit += tx + size*500;
}

//--------------------------------------------------------------------------
void EliminateOutOfLimits( TH1F* his, mdpdd* data ) 
{
  mdpdd* datanew = new mdpdd;
  CalculateHistoDims(his);
  mdpdd::iterator ite, ite2; 
  //  bool bOutOfLimitsPrev = true;
  //  rite2 = data->rbegin(); rite2++;
  for( ite = data->begin(); ite != data->end(); ) {
    if( verbose >= 3 ) std::cout << "EliminateOutOfLimits: " << (*ite).first << ": " << (*ite).second.first << " <? " << hyMin << std::endl; 
    if( (*ite).second.first < hyMin ) {
      /*      if( ite != data->begin() && !bOutOfLimitsPrev ) {
	//recalculate, interpolating with previous
	ite2 = ite; ite2--;
	double newX;
	if( !Definitions::bLogX && !Definitions::bLogY ) {
	  newX = (*ite2).first + ((*ite).first-(*ite2).first)/((*ite2).second.first-(*ite).second.first) * ((*ite2).second.first-hyMin);
	} else if( Definitions::bLogX && !Definitions::bLogY ) {
	  newX = exp( log((*ite2).first) + (log((*ite).first)-log((*ite2).first))/((*ite2).second.first-(*ite).second.first) * ((*ite2).second.first-hyMin));
	} else if( !Definitions::bLogX && Definitions::bLogY ) {
	  newX = (*ite2).first + ((*ite).first-(*ite2).first)/(log((*ite2).second.first)-log((*ite).second.first)) * (log((*ite2).second.first)-log(hyMin));
	} else if( Definitions::bLogX && Definitions::bLogY ) {
	  newX = exp( log((*ite2).first) + (log((*ite).first)-log((*ite2).first))/(log((*ite2).second.first)-log((*ite).second.first)) * (log((*ite2).second.first)-log(hyMin)));
	  if( verbose >= 3 ) std::cout << " EOOL newX " << newX 
					<< " " << log((*ite2).first) 
					<< " + " << log((*ite2).second.first)
					<< " - " << log((*ite).second.first)
					<< " / " << log((*ite).first)
					<< " - " << log((*ite2).first)
					<< " * " << log((*ite2).second.first)
					<< "- " << log(hyMin) 
					<< std::endl;

	}
	if( verbose >= 2 ) std::cout << verbose <<  " EOOL newX " << newX << std::endl;
	(*datanew)[newX] = pdd(hyMin,0.);
      } */
      ite2 = ite;
      ite++;
      data->erase(ite2);
      //      bOutOfLimitsPrev = true;
   // if( (*rite).second.first < hyMin ) {
    } else {
      ite++;
      //      bOutOfLimitsPrev = false;
    }
  }

  for( ite = datanew->begin(); ite != datanew->end(); ite++) {
    (*data)[(*ite).first] = (*ite).second;
  }
}
#endif


//--------------------------------------------------------------------------
void DrawGrid( TH1* histo, int nLinesX, int nLinesY )
{
  TAxis* xAxis = histo->GetXaxis();
  double xMin = xAxis->GetXmin();
  double xMax = xAxis->GetXmax();
  double yMin = histo->GetMinimum();
  double yMax = histo->GetMaximum();
  /*  int xNDiv = xAxis->GetNdivisions();
      int yNDiv = histo->GetYaxis()->GetNdivisions();*/
  int xNDiv = nLinesX;
  int yNDiv = nLinesY;
  double xDivStep = (xMax-xMin)/xNDiv;
  double yDivStep = (yMax-yMin)/yNDiv; 
  for( double fx = xMin+xDivStep; fx < xMax; fx+= xDivStep ){
    //    std::cout << " DRAWGRID " << fx << " " << xMin << " " << xMax << " " << xDivStep << std::endl; 
    TLine* line = new TLine( fx, yMin, fx, yMax );
    line->SetLineStyle(3);
    line->SetLineWidth(1);
    line->Draw();
  }
  for( double fy = yMin+yDivStep; fy < yMax; fy+= yDivStep ){
    //    std::cout << yNDiv << " DRAWGRID " << fy << " " << yMin << " " << yMax << " " << yDivStep << std::endl; 
    TLine* line = new TLine( xMin, fy, xMax, fy );
    line->SetLineStyle(3);
    line->SetLineWidth(1);
    line->Draw();
  }
}

//------------
void DrawGrid2( TH2* histo, int nLinesX, int nLinesY )
{
  TAxis* xAxis = histo->GetXaxis();
  double xMin = xAxis->GetXmin();
  double xMax = xAxis->GetXmax();
  TAxis* yAxis = histo->GetYaxis();
  double yMin = yAxis->GetXmin();
  double yMax = xAxis->GetXmax();
  /*  int xNDiv = xAxis->GetNdivisions();
      int yNDiv = histo->GetYaxis()->GetNdivisions();*/
  int xNDiv = nLinesX;
  int yNDiv = nLinesY;
  double xDivStep = (xMax-xMin)/xNDiv;
  double yDivStep = (yMax-yMin)/yNDiv;
  for( double fx = xMin+xDivStep; fx < xMax; fx+= xDivStep ){
    std::cout << " DRAWGRID2 " << fx << " " << xMin << " " << xMax << " " << xDivStep << std::endl; 
    TLine* line = new TLine( fx, yMin, fx, yMax );
    line->SetLineStyle(3);
    line->SetLineWidth(1);
    line->Draw();
  }
  for( double fy = yMin+yDivStep; fy < yMax; fy+= yDivStep ){
    std::cout << " DRAWGRID2 " << fy << " " << yMin << " " << yMax << " " << yDivStep << std::endl; 
    TLine* line = new TLine( xMin, fy, xMax, fy );
    line->SetLineStyle(3);
    line->SetLineWidth(1);
    line->Draw();
  }
}

