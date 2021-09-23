#include "TFile.h"
#include "TMarker.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLine.h"
#include "TCanvas.h"
#include <iostream>
#include <TStyle.h>
#include <map>
#include <set>
#include <math.h>
#include "ROOTUtilities/GetWordsInLine.C"
#include "ROOTUtilities/GetValue.C"
#include "ROOTUtilities/GenUtils.C"
#include "ROOTUtilities/Verbosity.C"
//#include "ReadProfMC.C"
TFile* hFile;
double theMultFactor;

typedef std::pair<double,double> pdd;
typedef std::multimap<double,double> mmdd;

double GetYValue( double xVal, mdpdd* values2 );
double theGMinX = -DBL_MAX;
double theGMaxX = DBL_MAX;
bool bDrawGGI = true;
//exeRoot getGammaIndex prod/dose_analyseSqdose.lowener.10.5.6.0..3..0..8628_ALL.root
//double startXGI = 174;
double startXGI = 0.;
double theDistStep = 0.1/10.; // distance step to check gamma 
double MAX_DIST_LOOP = 10.; // MAX DISTANCE TO CHECK GAMMA
double theHGILim = 1000.;
double theGIPVal = 1.;
double theGIAbovePVal = 0.;
std::multiset<double> theGammaFinal;
TH1F* histoGIG;

typedef std::map<double,double>::iterator mdd;
TH1F* getGammaIndex1CD( mdpdd* dataVal, mdpdd* mcVal, double perCentLim, double minDistLim, double pval = 1. )
{
  verbose = 1;
  theGIPVal = pval;
  if( verbose >= 2 )  std::cout << "getGammaIndex1CD NP= " << dataVal->size() << " " << mcVal->size() << " " << perCentLim << " / " << minDistLim << std::endl;

  if( dataVal->size() == 0 || mcVal->size() == 0 ) {
    std::cerr << " !!!!!! getGammaIndex1CD  first set or second set have no entries !!! " << std::endl;
    exit(1);
  }
  theGammaFinal.clear();

  //---- Get data maximum
  mdpdd::iterator itec1;
  double dataMax = 0.;
  for( itec1 = dataVal->begin(); itec1 != dataVal->end(); itec1++ ) {
    dataMax = std::max(dataMax,(*itec1).second.first);
  }
  
  if( verbose >= 2 ) std::cout << "LOOPING perCent " << perCentLim << " minDist " << minDistLim << " DataMax " << dataMax << std::endl;
  if( verbose >= 2 ) std::cout << "X LIMITS " << theGMinX << " " << theGMaxX << std::endl;
  if( theGMaxX == theGMinX ) {
    std::cerr << " !!!!!! getGammaIndex1CD  theGMaxX = theGMinX " << theGMinX << std::endl;
    exit(2);
  }
  std::string limstr = "getgammaIndex:"+ftoa(perCentLim)+":"+ftoa(minDistLim);
  TH1F* histoGI = new TH1F(limstr.c_str(),limstr.c_str(),100,0.,theHGILim);
  limstr += "_GAMMA";
  double minX = (*(mcVal->begin())).first;
  double maxX = (*(mcVal->rbegin())).first;
  int nBinsX = mcVal->size();
  histoGIG = new TH1F(limstr.c_str(),limstr.c_str(),nBinsX,minX,maxX);
  int nGI = 0;
  int nPOINTS = 0;    
  double nData = dataVal->size();
  for( itec1 = dataVal->begin(); itec1 != dataVal->end(); itec1++ ) {
    double xValData = (*itec1).first;
    if( xValData < theGMinX || xValData > theGMaxX ) continue;
    double yValData = (*itec1).second.first;
    //    double yErrData = (*itec1).second.second;
      //---- Get MC value at same X    
    double yValMC = GetYValue( xValData, mcVal );
    double perCent = fabs(yValData - yValMC)*100./dataMax; //REL OR ABS??
    //double perCent = fabs(yValData - yValMC)*100./(yValData);
    if( verbose >= 2) std::cout << " " << xValData << " : perCent " << perCent << " g= " << perCent/perCentLim << " = " << yValData << " <-> " << yValMC << std::endl;
    
    double minGamma = DBL_MAX;
    //--- Get gamma from percent 
    double gammaPC = perCent/perCentLim;
    minGamma = gammaPC;
    double distLoop = gammaPC*minDistLim; // further than this it will make gamma bigger
    if( distLoop > MAX_DIST_LOOP ) distLoop = MAX_DIST_LOOP;
    if( verbose >= 2 ) std::cout << " @@ distMaxLoop " << distLoop << " gammaPC " << gammaPC << " " << minDistLim << std::endl;
    gammaPC *= gammaPC;
    for( double id = theDistStep; id < distLoop; id += theDistStep ) {
      yValMC = GetYValue( xValData+id, mcVal );
      perCent = (yValData - yValMC)*100./(yValData);
      double gamma = sqrt((perCent/perCentLim)*(perCent/perCentLim)+id*id);
      //      if( verbose >= 3 ) std::cout << " gamma= " << gamma << " = sqrt( " << perCent/perCentLim*(perCent/perCentLim)<< " + " << id*id << " )" << std::endl;
      minGamma = std::min(minGamma,gamma);
      if( verbose >= 3) std::cout << "g= " << gamma << " " << xValData<< " + " <<  id << " minGamma " << minGamma << " perCent " << perCent << " = " << yValData << " - " << yValMC << std::endl;
    }
    for( double id = theDistStep; id < distLoop; id += theDistStep ) {
      yValMC = GetYValue( xValData-id, mcVal );
      perCent = (yValData - yValMC)*100./(yValData);
      double gamma = sqrt((perCent/perCentLim)*(perCent/perCentLim)+id*id);
      minGamma = std::min(minGamma,gamma);
      if( verbose >= 3) std::cout << "g= " << gamma << " " << xValData<< " - " <<  id << " minGamma " << minGamma << " perCent " << perCent << " = " << yValData << " - " << yValMC << std::endl;
    }
    histoGI->Fill(minGamma);
    histoGIG->Fill(xValData,minGamma);
    if( minGamma > 1. ) nGI++;
    nPOINTS++;
      theGammaFinal.insert(std::multiset<double>::value_type(minGamma));
      if( verbose >= 2) std::cout << "FINAL gamma " << xValData << " = " << minGamma << "N" << theGammaFinal.size() << std::endl;
  
  }
  if(verbose >= 1) std::cout << perCentLim << "% / " << minDistLim << "mm : N POINTS OUTSIDE " << nGI << " out of " << nPOINTS << "N" << theGammaFinal.size()<< std::endl;
  
  if( bDrawGGI ) {
    gStyle->SetOptStat(1111111);
    theCanvas->cd();
    histoGI->Draw();
    theCanvas->Print(("00"+std::string(histoGI->GetName())+".gif").c_str());
    histoGIG->Draw();
    theCanvas->Print(("00"+std::string(histoGIG->GetName())+".gif").c_str());
    if( verbose >= 1 ) std::cout << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGI->GetMean() << " +- " << histoGI->GetRMS() << std::endl;
  }

  //---- Get gamma where fraction of data is > theGIPVal
  std::multiset<double>::const_iterator itep;
  double nEntGood = 0;
  for( itep = theGammaFinal.begin(); itep != theGammaFinal.end(); itep++, nEntGood += 1./nData ) {
    if( nEntGood > theGIPVal ) break;
  }
  if( theGammaFinal.size() > 0 ) itep--;
  theGIAbovePVal = *itep;

  return histoGI;
  
}

double GetYValue( double xVal, mdpdd* values2 )
{
  double yVal;
  mdpdd::iterator itec1,itec2;
  //  double minX = (*(values2->begin())).first;
  //  double maxX = (*(values2->rbegin())).first;
  //  if( xVal < minX || xVal > maxX ) return 0.;
  itec1 = values2->find(xVal);
  double valXD = 0.; 
  double valXU = 0.;
  if( itec1 == values2->end() ) {
    // interpolate linearly between values
    itec2 = values2->lower_bound(xVal); 
    if( itec2 == values2->begin() ) {
      itec2++;
      itec1 = itec2; 
      itec1--;
      valXD = xVal; 
      valXU = (*itec2).first; 
    } else if( itec2 == values2->end() ){
      itec2--;
      itec1 = itec2;
      itec1--;
      valXD = (*itec1).first ; 
      valXU = xVal;
    } else {
      itec1 = itec2;
      itec1--;
      valXD = (*itec1).first ; 
      valXU = (*itec2).first ; 
    }
    double slopeVal = 0.;
    if( itec2 != itec1 ) {
      slopeVal = ((*itec2).second.first - (*itec1).second.first ) / ( valXU - valXD );
    }
    yVal = (*itec1).second.first + slopeVal * ( xVal - (*itec1).first );
    //      std::cout << xVal << " COMPARE INTERP " << yVal
    //      << " between X= " << (*itec1).first << " - " << (*itec2).first 
    //      << " valX= " << (*itec1).second.first << " - " << (*itec2).second.first
    //      << " slope " << slopeVal 
    //      <<	std::endl; 
  } else {
    yVal = (*itec1).second.first;
    itec2 = itec1; //for below
  }
 
  return yVal; 
}
