#ifndef GmHisto1_hh
#define GmHisto1_hh
#ifdef GAMOS_NO_ROOT
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <string>
#include <map>
#include <iostream>
#include <math.h>

class GmHisto1
{
public:
  GmHisto1( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  GmHisto1( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge );
  ~GmHisto1(){};
  
  void Fill( double value, double weight = 1. );
  
  std::string GetName() const { return theName; }
  int GetNbins() const { return theNBins; }
  int GetNbinsX() const { return theNBins; }
  double GetLowerEdge() const { return theLowerEdge; }
  double GetUpperEdge() const { return theUpperEdge; }
  double GetBinWidth() const { return theBinWidth; }
  double GetBinWidth( int ) const { return theBinWidth; }
  double GetBinLowEdge( int ibin ) const;
  double GetBinUpEdge( int ibin ) const;
  double GetBinCenter( int ibin ) const;
  
  double GetBinContent(int ibin) const;
  double GetBinError(int ibin) const;
  void SetBinContent(int ibin, double val);
  void SetBinError(int ibin, double val);
  void AddBinContent(int ibin, double val);
  
  double GetSumW() const { return theSumW;}
  double GetSumW2() const { return theSumW2;}
  double GetSumWX() const { return theSumWX;}
  double GetSumWX2() const { return theSumWX2;}
  
  double GetBinSumW(int ibin) const { 
    std::map<int,double>::const_iterator ite = theBinSumW.find(ibin);
    return (*ite).first;}
  double GetBinSumW2(int ibin) const {
    std::map<int,double>::const_iterator ite = theBinSumW2.find(ibin);
    return (*ite).first;}
    
  int GetEntries() const;
  void SetEntries(int ent ) { theEntries = ent;}
  double GetMean() const;
  double GetRMS() const;
  double GetMeanError() const;
  double GetRMSError() const;
  
  double GetIntegral() const;
  int GetBin(double binx ) const;
  
  //  virtual Float_t GetKurtosis(Int_t axis=1) const;
  //    virtual Float_t GetSkewness(Int_t axis=1) const;
  //  virtual Float_t GetMaximum(Float_t maxval=FLT_MAX) const;
  //   virtual Int_t    GetMaximumBin() const;
  //   virtual Float_t GetMinimum(Float_t minval=-FLT_MAX) const;
  //   virtual Int_t    GetMinimumBin() const;
  double GetEffectiveEntries() const;

protected:
  std::string theName;
  int theNBins;
  double theLowerEdge;
  double theUpperEdge;
  double theBinWidth;

  int theEntries; 
  std::map<int,int> theBinN;
  std::map<int,double> theBinSumW;
  std::map<int,double> theBinSumW2;

  double theSumW;
  double theSumW2;
  double theSumWX;
  double theSumWX2;

  friend  GmHisto1 operator+(const GmHisto1 &h1, const GmHisto1 &h2);
  
};

#endif

#endif
