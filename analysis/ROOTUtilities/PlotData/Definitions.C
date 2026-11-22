#ifndef Definitions_C
#define Definitions_C

#include "TCanvas.h"
#include <map>
#include <float.h>

//typedef std::map<double,double> mdd; // energy - value 
//typedef std::map <std::string,mdd* > msmdd; 
typedef std::pair<double,double> pdd; // value, error
typedef std::map <double,pdd > mdpdd;  // energy - value, error
typedef std::map <std::string,mdpdd* > msmdpdd; 

class Definitions {

public:
  static double xDataMin;
  static double xDataMax;
  static double yDataMin;
  static double yDataMax;
  static bool bLogX;
  static bool bLogY;
  // total extension of histogram pad 
  static double TOTAL_WIDTH; 
  static double TOTAL_HEIGTH;
  
  //float minHisX = 1.E-20;
  static std::string xAxisTitle;
  static std::string yAxisTitle;
  static double xAxisTitleOffset;
  static double yAxisTitleOffset;
  static double xAxisTitleSize;
  static double yAxisTitleSize;
  static double xAxisLabelSize;
  static double yAxisLabelSize;
  static double xAxisLabelOffset;
  static double yAxisLabelOffset;
  static double markerSize;
  static double markerTextDispX;
  static double markerTextDispY;
  static double markerTextSize;
  static bool lineJoinMarker;
  static double canvasSizeX;
  static double canvasSizeY;
  static double padRightMargin;
  static double padLeftMargin;
  static double xHistoLabel;
  static double yHistoLabel;
};

double Definitions::xDataMin = DBL_MAX;
double Definitions::xDataMax = -DBL_MAX;
double Definitions::yDataMin = DBL_MAX;
double Definitions::yDataMax = -DBL_MAX;
bool Definitions::bLogX = 0;
bool Definitions::bLogY = 0;
double Definitions::canvasSizeX = 750; // NORMAL = 500;
double Definitions::canvasSizeY = 750; // NORMAL = 500;
double Definitions::TOTAL_WIDTH = 540*Definitions::canvasSizeX/500; 
double Definitions::TOTAL_HEIGTH = 390*Definitions::canvasSizeY/500; 
std::string Definitions::xAxisTitle = "";
std::string Definitions::yAxisTitle = "";
double Definitions::xAxisTitleOffset = 1.;
double Definitions::yAxisTitleOffset = 1.;
double Definitions::xAxisTitleSize = 0.04;
double Definitions::yAxisTitleSize = 0.04;
double Definitions::xAxisLabelSize = 0.04;
double Definitions::yAxisLabelSize = 0.04;
double Definitions::xAxisLabelOffset = 0.;
double Definitions::yAxisLabelOffset = 0.;
double Definitions::markerSize = 0.5;
double Definitions::markerTextDispX = 5.;
double Definitions::markerTextDispY = 1.;
double Definitions::markerTextSize = 0.02;
bool Definitions::lineJoinMarker = 1;
double Definitions::padRightMargin = 0.;
double Definitions::padLeftMargin = 0.;
double Definitions::xHistoLabel = 0.;
double Definitions::yHistoLabel = 0.;

TCanvas* theCanvas = new TCanvas("dd");
//theCanvas->SetCanvasSize(theCanvasSizeX,theCanvasSizeY);

#endif
