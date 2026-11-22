#ifndef GmHisto2_hh
#define GmHisto2_hh
#ifndef GAMOS_ROOT
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <string>
#include <vector>
#include "G4ios.hh"
#include <map>
#include <iostream>
#include <math.h>

//------------------------------------------------------------------------
// 2D histogram used when GAMOS is compiled without ROOT (GAMOS_ROOT not defined).
// It reproduces the behaviour of ROOT TH2F (ROOT 6.26), that is used when GAMOS is compiled with ROOT (see GmHisto1.hh)
// Global bin number is binX + (nbinsX+2)*binY, bin 0 is the underflow and bin nbins+1 the overflow in each axis
//------------------------------------------------------------------------
class GmHisto2
{
public:
  GmHisto2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHisto2( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHisto2( const GmHisto2& his ) = default;
  GmHisto2& operator=( const GmHisto2& his ) = default;
  virtual ~GmHisto2(){};

  enum { kNstat = 13 }; // size of statistics array (as ROOT TH1::kNstat)

  virtual int Fill( double valueX, double valueY );
  virtual int Fill( double valueX, double valueY, double weight );

  std::string GetName() const { return theName; }
  int GetNbinsX() const { return theNBinsX; }
  int GetNbinsY() const { return theNBinsY; }
  double GetLowerEdgeX() const { return theLowerEdgeX; }
  double GetLowerEdgeY() const { return theLowerEdgeY; }
  double GetUpperEdgeX() const { return theUpperEdgeX; }
  double GetUpperEdgeY() const { return theUpperEdgeY; }
  double GetBinWidthX() const { return theBinWidthX; }
  double GetBinWidthY() const { return theBinWidthY; }
  double GetBinWidthX(int ) const { return theBinWidthX; }
  double GetBinWidthY(int ) const { return theBinWidthY; }
  double GetBinLowEdgeX( int ibin ) const;
  double GetBinUpEdgeX( int ibin ) const;
  double GetBinLowEdgeY( int ibin ) const;
  double GetBinUpEdgeY( int ibin ) const;
  double GetBinCenterX( int ibin ) const;
  double GetBinCenterY( int ibin ) const;
  int FindBinX( double valueX ) const;
  int FindBinY( double valueY ) const;
  int GetBinX(float binx ) const; // same as FindBinX, but prints an error if value is outside limits
  int GetBinY(float biny ) const; // same as FindBinY, but prints an error if value is outside limits
  int GetBin( int ibinX, int ibinY ) const; // global bin number

  virtual double GetBinContent(int ibinX, int ibinY) const;
  virtual double GetBinError(int ibinX, int ibinY) const;
  virtual void SetBinContent(int ibinX, int ibinY, double val);
  virtual void SetBinError(int ibinX, int ibinY, double val);
  virtual void AddBinContent(int ibin); // global bin number
  virtual void AddBinContent(int ibin, double val); // global bin number
  virtual void Scale(double fac); // as ROOT TH1::Scale: multiply contents and errors by fac, keeping means, RMSs and effective entries
  virtual bool Add(const GmHisto2* his, double fac = 1.); // as ROOT TH1::Add
  virtual void Sumw2( bool flag = true );
  int GetSumw2N() const { return int(theSumw2.size()); }

  double GetSumW() const { return theTsumw;}
  double GetSumW2() const { return theTsumw2;}
  double GetSumWX() const { return theTsumwx;}
  double GetSumWX2() const { return theTsumwx2;}
  double GetSumWY() const { return theTsumwy;}
  double GetSumWY2() const { return theTsumwy2;}

  double GetEntries() const { return theEntries; }
  void SetEntries( double ent ) { theEntries = ent;}

  double Integral() const;  // sum of bin contents, without underflow and overflow
  double GetIntegral() const { return Integral(); }

  double GetBinSumW(int ibin) const;  // bin content as stored (global bin number)
  double GetBinSumW2(int ibin) const; // bin error squared (global bin number)

  double GetEffectiveEntries() const;

  double GetMean( int axis = 1 ) const;
  double GetRMS( int axis = 1 ) const { return GetStdDev(axis); }
  double GetStdDev( int axis = 1 ) const;
  double GetMeanError( int axis = 1 ) const { return GetMean(axis+10); }
  double GetRMSError( int axis = 1 ) const { return GetStdDev(axis+10); }
  double GetStdDevError( int axis = 1 ) const { return GetStdDev(axis+10); }

  virtual void GetStats(double* stats) const;
  virtual void PutStats(double* stats);
  void ResetStats();

  virtual void Reset();

protected:
  virtual double GetBinContent(int ibin) const; // global bin number
  virtual double GetBinError(int ibin) const; // global bin number
  virtual void SetBinContent(int ibin, double val); // global bin number
  virtual void SetBinError(int ibin, double val); // global bin number
  virtual double RetrieveBinContent(int ibin) const { return theArray[ibin]; }
  virtual void UpdateBinContent(int ibin, double val) { theArray[ibin] = double(float(val)); } // stored as float, as TH2F
  virtual double GetBinErrorSqUnchecked(int ibin) const { return theSumw2.size() ? theSumw2[ibin] : RetrieveBinContent(ibin); }
  void CheckLimits();

protected:
  std::string theName;
  int theNBinsX;
  double theLowerEdgeX;
  double theUpperEdgeX;
  int theNBinsY;
  double theLowerEdgeY;
  double theUpperEdgeY;
  double theBinWidthX;
  double theBinWidthY;
  int theNcells; // number of bins including underflows and overflows

  std::vector<double> theArray; // bin contents
  std::vector<double> theSumw2; // sum of weights squared per bin (empty if not created)

  double theEntries;
  double theTsumw;
  double theTsumw2;
  double theTsumwx;
  double theTsumwx2;
  double theTsumwy;
  double theTsumwy2;
  double theTsumwxy;

};

GmHisto2 operator+(const GmHisto2 &h1, const GmHisto2 &h2);

#endif

#endif
