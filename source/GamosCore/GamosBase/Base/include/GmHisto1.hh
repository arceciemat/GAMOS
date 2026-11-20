#ifndef GmHisto1_hh
#define GmHisto1_hh
#ifdef GAMOS_NO_ROOT

#include <string>
#include <vector>
#include "G4ios.hh"
#include <math.h>
#include <cfloat>

//------------------------------------------------------------------------
// 1D histogram used when GAMOS is compiled without ROOT (GAMOS_NO_ROOT).
// It reproduces the behaviour of ROOT TH1F (ROOT 6.26), that is used when GAMOS is compiled with ROOT:
//  - bin contents are stored in float precision, statistics in double precision
//  - bin 0 is the underflow and bin nbins+1 the overflow; they are not used in the statistics
//  - bin errors are sqrt(|content|) until the sum of squares of weights per bin is created (Sumw2()),
//    which is done automatically when filling with a weight != 1, setting a bin error, scaling or adding a histogram that has it
//  - after SetBinContent the statistics (mean, RMS, effective entries) are recalculated from the bin contents at the bin centres
//------------------------------------------------------------------------
class GmHisto1
{
public:
  GmHisto1( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  GmHisto1( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge );
  GmHisto1( const GmHisto1& his ) = default;
  GmHisto1& operator=( const GmHisto1& his ) = default;
  virtual ~GmHisto1(){};

  enum { kNstat = 13 }; // size of statistics array (as ROOT TH1::kNstat)

  virtual int Fill( double value );
  virtual int Fill( double value, double weight );

  std::string GetName() const { return theName; }
  int GetNbins() const { return theNBins; }
  int GetNbinsX() const { return theNBins; }
  double GetLowerEdge() const { return theLowerEdge; }
  double GetUpperEdge() const { return theUpperEdge; }
  double GetLowEdge() const { return theLowerEdge; }
  double GetUppEdge() const { return theUpperEdge; }
  double GetBinWidth() const { return theBinWidth; }
  double GetBinWidth( int ) const { return theBinWidth; }
  double GetBinLowEdge( int ibin ) const;
  double GetBinUpEdge( int ibin ) const;
  double GetBinCenter( int ibin ) const;
  int FindBin( double value ) const;
  int GetBin(double binx ) const; // same as FindBin, but prints an error if value is outside limits

  virtual double GetBinContent(int ibin) const;
  virtual double GetBinError(int ibin) const;
  virtual void SetBinContent(int ibin, double val);
  virtual void SetBinError(int ibin, double val);
  virtual void AddBinContent(int ibin);
  virtual void AddBinContent(int ibin, double val);
  virtual void Scale(double fac); // as ROOT TH1::Scale: multiply contents and errors by fac, keeping mean, RMS and effective entries
  virtual bool Add(const GmHisto1* his, double fac = 1.); // as ROOT TH1::Add
  virtual void Sumw2( bool flag = true );
  int GetSumw2N() const { return int(theSumw2.size()); }

  double GetSumW() const { return theTsumw;}
  double GetSumW2() const { return theTsumw2;}
  double GetSumWX() const { return theTsumwx;}
  double GetSumWX2() const { return theTsumwx2;}

  double GetBinSumW(int ibin) const;  // bin content as stored
  double GetBinSumW2(int ibin) const; // bin error squared

  double GetEntries() const { return theEntries; }
  void SetEntries(double ent ) { theEntries = ent;}
  double GetMean(int axis = 1) const;
  double GetRMS(int axis = 1) const { return GetStdDev(axis); }
  double GetStdDev(int axis = 1) const;
  double GetMeanError(int axis = 1) const { return GetMean(axis+10); }
  double GetRMSError(int axis = 1) const { return GetStdDev(axis+10); }
  double GetStdDevError(int axis = 1) const { return GetStdDev(axis+10); }

  double Integral() const;  // sum of bin contents, without underflow and overflow

  //--- as ROOT TH1: maximum/minimum bin content (without underflow and overflow), unless set by SetMaximum/SetMinimum
  double GetMaximum( double maxval = FLT_MAX ) const; // maximum of contents < maxval
  double GetMinimum( double minval = -FLT_MAX ) const; // minimum of contents > minval
  int GetMaximumBin() const;
  int GetMinimumBin() const;
  void SetMaximum( double maximum = -1111 ) { theMaximum = maximum; } // -1111: not set (as ROOT)
  void SetMinimum( double minimum = -1111 ) { theMinimum = minimum; }
  double GetIntegral() const { return Integral(); }

  double GetEffectiveEntries() const;

  virtual void GetStats(double* stats) const;
  virtual void PutStats(double* stats);
  void ResetStats();

  virtual void Reset();
  bool UseErrors() const {
    return theSumw2.size() != 0;
  }
  void SetUseErrors( bool val ) {
    Sumw2(val);
  }

protected:
  virtual double RetrieveBinContent(int ibin) const { return theArray[ibin]; }
  virtual void UpdateBinContent(int ibin, double val) { theArray[ibin] = double(float(val)); } // stored as float, as TH1F
  virtual double GetBinErrorSqUnchecked(int ibin) const { return theSumw2.size() ? theSumw2[ibin] : RetrieveBinContent(ibin); }
  void CheckLimits();

protected:
  std::string theName;
  int theNBins;
  double theLowerEdge;
  double theUpperEdge;
  double theBinWidth;
  int theNcells; // number of bins including underflow and overflow

  std::vector<double> theArray; // bin contents
  std::vector<double> theSumw2; // sum of weights squared per bin (empty if not created)

  double theEntries;
  double theTsumw;
  double theTsumw2;
  double theTsumwx;
  double theTsumwx2;

  double theMaximum; // set by SetMaximum (-1111: not set, as ROOT)
  double theMinimum; // set by SetMinimum (-1111: not set, as ROOT)
};

GmHisto1 operator+(const GmHisto1 &h1, const GmHisto1 &h2);

#endif

#endif
