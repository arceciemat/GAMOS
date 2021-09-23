#ifndef plotActivation_C
#define plotActivation_C

#include "readElementZ.C"
#include "readExemption.C"

#include "TFile.h"
#include "TH1F.h"
#include "TMarker.h"
#include "TPad.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"
#include "TColor.h"
#include <math.h>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>

void DrawHistos(int nHistos);
void readActivityResults(const char* fileName);
//void readActivityResultsAverage3(const char* fileName);
#include "ROOTUtilities/Activation/IsotActivation.cc"
std::map<std::string,IsotActivation*> theIsotopes;

TCanvas* theCanvas;
int verbose;
std::vector<double> theTimes;
typedef std::map<double,double> mdd;
typedef std::pair< std::string, mdd > psmdd;
typedef std::vector< psmdd > vpsmdd;
vpsmdd theActivitiesPerIsotope;
double valueMin, valueMax;
std::string theUnitName;
double theUnitValue;
double hMaxY;
double hMinY;
typedef std::pair< std::string, double > psd;

//------------------------------------------------------------------------
void plotActivation(const char* fileName, const char* unit = "seconds" )
{
  hMinY = -5;

  theUnitName = unit;
  if( theUnitName == "seconds" ) {
    theUnitValue = 1.;
  } else if( theUnitName == "seconds" ) {
    theUnitValue = 1.;
  } else if( theUnitName == "minutes" ) {
    theUnitValue = 60.;
  } else if( theUnitName == "hours" ) {
    theUnitValue = 3600.;
  } else if( theUnitName == "days" ) {
    theUnitValue = 86400.;
  } else if( theUnitName == "years" ) {
    theUnitValue = 365*86400.;
  } else {
    std::cerr << " NOT VALID UNIT NAME " << unit << std::endl;
  }

  verbose = 2;
  valueMin = DBL_MAX;
  valueMax = -DBL_MAX;

  theTimes.push_back(1.); 
  theTimes.push_back(60.);
  theTimes.push_back(3600.);
  theTimes.push_back(86400.);
  theTimes.push_back(7.*86400.);
  theTimes.push_back(30.*86400.);
  theTimes.push_back(365.*86400.);
  theTimes.push_back(3650.*86400.);

  //  readElementZ();
  //  readExemption();

  readActivityResults(fileName);
  //  readActivityResultsAverage3(fileName);

  DrawHistos(10);

}

//------------------------------------------------------------------------
void readActivityResults(const char* fileName)
{
  std::vector <std::string> wl;
  hMinY = DBL_MAX;
  hMaxY = 0.;

  std::ifstream fin(fileName); 
  if( !fin.is_open() ) {
    std::cout << "!!! ERROR : FILE NOT FOUND " << fileName << std::endl;
    exit(1);
  }

  int ii = 0;
  //%%%%%% SHActivationUA_inCameraF:  NUMBER OF EVENTS PER SECOND = 2.28311e+10 = 1e+07 events in 438000 nanoseconds 
  //%%% SHActivationUA_inCameraF: END_PRODUCTION: 3.1536e+08 sec  TIME: 2.592e+06 sec : Nb93[30.8] : ACTIVITY= 490.959 decays/sec   LIFETIME=7.34332e+08 sec   NPART_IN_JOB= 88   NIsotopes= 5.20131e+11 
  IsotActivation* isot = 0;
  for(;;ii++) {
    if( !GetWordsInLine(fin,wl) ) break;
    //    std::cout << " LINE " << wl.size() << " " << wl[0] << std::endl;
    if( wl.size() > 1 && wl[1].find("SHActivationUA") != std::string::npos ) {
      if( wl.size() > 5 && wl[2] == "NUMBER_OF_EVENTS_PER_SECOND" ) {
	IsotActivation::theNeventsPerSecond = int(atof( wl[8].c_str()));
    std::cout << " LINE SETTING NEVENTS " << wl.size() << " " << wl[8] << std::endl;
      } else if( wl.size() > 10 && wl[6] == "ACTIVITY=" ) {
	std::cout << " ADDING ISOT " << wl.size() << " " << wl[5] << std::endl;
	std::string isotName = wl[5];
	if( isotName == "neutron" ) continue;
	std::map<std::string,IsotActivation*>::const_iterator ites = theIsotopes.find( isotName );
	if( ites == theIsotopes.end() ) {
	  isot = new IsotActivation();
	  theIsotopes[isotName] = isot;
	} else {
	  isot = (*ites).second;
	}
	isot->AddData( wl );
	hMinY = std::min(hMinY,isot->GetMinimumActivity());
	hMaxY = std::max(hMaxY,isot->GetMaximumActivity());
      }
    }
  }

}


void DrawHistos( int nHistosPerSheet )
{
  theCanvas = new TCanvas;
  gStyle->SetOptStat(0);
  theCanvas->Draw();

  // get minimum and maximum time
  double timeMin = DBL_MAX;
  double timeMax = 0;
  std::set<double>::const_iterator itet;
  for( itet = IsotActivation::theTimes.begin(); itet != IsotActivation::theTimes.end(); itet++ ) {
    double time = *itet;
    timeMin = std::min(time,timeMin);
    timeMax = std::max(time,timeMax);
  }

  // draw empty histogram 
  double hMinX = log10( timeMin / pow(timeMax/timeMin,0.1) );
  double hMaxX = log10( timeMax * pow(timeMax/timeMin,0.2) );
  double hWidthX = hMaxX/hMinX;
  double hw = pow(hMaxY/hMinY,0.3); 
  hMinY = log10( hMinY / hw ); 
  hMaxY = log10( hMaxY * hw );
  double hWidthY = hMaxY/hMinY;

  TH1F* his = new TH1F("ISOTOPE ACTIVITY","ISOTOPE ACTIVITY", 100, hMinX, hMaxX);
  if( verbose >= 1)  std::cout << timeMax << " X MIN " << hMinX << " H MAX " << hMaxX 
			       << " Y MIN " << hMinY << " Y MAX " << hMaxY << std::endl;
  his->SetMaximum( hMaxY );
  //  his->SetMinimum( log10(valueMin) -1 );
  his->SetMinimum( hMinY );

  std::map<std::string,IsotActivation*>::const_iterator ite;
  int ii = 0;
  std::string printedIsotNames = "";

  for( ite = theIsotopes.begin(); ite != theIsotopes.end(); ite++, ii++ ) {
    IsotActivation* isot = (*ite).second;

    std::string isotName = (*ite).first;

    // set colour
    int icolour;
    int ih = ii % nHistosPerSheet;
    if( ih <= 3 ) { // 5 is yellow
      icolour = ih+1; 
    } else if( ih <= 7 ) { // 10 is white
      icolour = ih+2; 
    } else {
      icolour = ih+3; 
    }    
    std::cout << " COLOUR " << icolour << " " << isotName << std::endl;

    if( ih == 0 ) {
      if( ii != 0 ) {
	char buffer[50];
	sprintf(buffer,"%i",int((ii-1)/nHistosPerSheet+1));
	std::string fname = "activity."+std::string(buffer)+"."+printedIsotNames+".gif";
	theCanvas->cd();
	theCanvas->Print(fname.c_str());
      }
      printedIsotNames = "";
      his->Draw();
      TText* unitText = new TText( hMaxX/pow(hWidthX,0.5),hMinY/pow(hWidthY,0.1), theUnitName.c_str() );
      unitText->SetTextSize(0.05);
      unitText->Draw();

    }
    printedIsotNames += isotName + ".";

    std::map<double,double>::const_iterator itea, iteaprev;
    double lastActivity = 0;
    for( itea = isot->theActivities.begin(); itea != isot->theActivities.end(); itea++ ) {
      double x1 = log10((*itea).first/theUnitValue);
      double y1 = log10((*itea).second);
      lastActivity = y1;
      std::cout << " PLOT X " << x1 << " Y " << y1 << std::endl;
      TMarker *mark = new TMarker( x1, y1, 20+ih );
      mark->SetMarkerColor(icolour);
      mark->SetMarkerSize(0.7);
      mark->Draw();
      
      // Draw exemption line
      std::map<std::string,double>::const_iterator iteExem = theExemption.find(isotName);
      //      std::cout << " EXEMPTION " << theExemption.size() << " " << isotName << std::endl;
      double exemption = DBL_MAX;
      if( iteExem != theExemption.end() ){
	exemption = log10((*iteExem).second);
	TLine *exemLine = new TLine(hMinX, exemption, hMaxX, exemption );
	exemLine->SetLineColor(icolour);
	exemLine->SetLineStyle(2);
	exemLine->SetLineWidth(1);
	exemLine->Draw();
      }
      
      // Draw line joining markers
      if( itea != isot->theActivities.begin() ) {
	iteaprev = itea; iteaprev--;
	double x2 = log10((*iteaprev).first/theUnitValue);
	double y2 = log10((*iteaprev).second);
	TLine *line = new TLine(x1, y1, x2, y2);
	line->SetLineColor(icolour);
	line->Draw();
	// Get intersection with exemption line
	if( exemption != DBL_MAX && y1 != y2 ) {
	  double inters = (exemption-y1)/(y2-y1)*(x2-x1)+x1;
	  // check is in line segment
	  std::cout << isotName << " INTERS " << inters << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " exem " << exemption << std::endl;
	  if( inters < x1 && inters > x2 ){
	    TLine *exemVLine = new TLine(inters, exemption, inters, hMinY );
	    exemVLine->SetLineColor(icolour);
	    exemVLine->SetLineStyle(2);
	    exemVLine->SetLineWidth(1);
	    exemVLine->Draw();
	    char buffer[50];
	    sprintf(buffer,"%f",pow(10.,inters));
	    TText* intersText = new TText( inters,hMinY/pow(hWidthY,0.4),buffer );
	    intersText->SetTextColor(icolour);
	    intersText->SetTextSize(0.03);
	    intersText->Draw();

	  }
	}
      }
    }

    double lt = isot->theLifeTime/theUnitValue;
    char ltStr[50];
    sprintf(ltStr,"%f",lt);
    std::string textName = isotName + " " + std::string(ltStr);
    std::cout << " TEXT isot " << hMaxX/pow(hWidthX,0.1) << " " << lastActivity << " " << textName.c_str() << std::endl;
    double tx = hMinX*pow(hWidthX,ii%nHistosPerSheet*0.1);
    double ty = log10(isot->GetActivity( pow(10.,tx) ));
    TText* isotText = new TText( tx,ty, textName.c_str() );
    isotText->SetTextColor(icolour);
    isotText->SetTextSize(0.02);
    isotText->Draw();

  }

  char buffer[50];
  sprintf(buffer,"%i",int((ii-1)/nHistosPerSheet+1));
  std::string fname = "activity."+std::string(buffer)+"."+printedIsotNames+".gif";
  theCanvas->cd();
  theCanvas->Print(fname.c_str());

}

#endif
