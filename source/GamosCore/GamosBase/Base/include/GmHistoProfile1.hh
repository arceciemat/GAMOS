#ifndef GmHistoProfile1_hh
#define GmHistoProfile1_hh
#ifndef GAMOS_ROOT

#include "GmHisto1.hh"
#include <iostream>

//------------------------------------------------------------------------
// 1D profile histogram used when GAMOS is compiled without ROOT (GAMOS_ROOT not defined).
// It reproduces the behaviour of ROOT TProfile (ROOT 6.26) with default error option (error on the mean), that is used when GAMOS is compiled with ROOT:
//  for each bin it stores sum(w*y) (in theArray), sum(w*y*y) (in theSumw2), sum(w) (in theBinEntries) and sum(w*w) (in theBinSumw2, only once a weight != 1 is used)
//  bin content is the mean of y: sum(w*y)/sum(w)
//------------------------------------------------------------------------
class GmHistoProfile1 : public GmHisto1
{
public:
  GmHistoProfile1( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  GmHistoProfile1( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge );
  GmHistoProfile1( const GmHistoProfile1& his ) = default;
  GmHistoProfile1& operator=( const GmHistoProfile1& his ) = default;
  virtual ~GmHistoProfile1(){};

  virtual int Fill( double ) { G4cerr << "Error in <GmHistoProfile1::Fill(double)>: may not use (as TProfile)" << std::endl; return -1; }
  virtual int Fill( double valueX, double valueY );
  virtual int Fill( double valueX, double valueY, double weight );

  virtual double GetBinContent(int ibin) const;
  virtual double GetBinError(int ibin) const;
  double GetBinEntries(int ibin) const;
  double GetBinEffectiveEntries(int ibin) const;
  virtual void AddBinContent(int ibin);
  virtual void AddBinContent(int ibin, double val);
  virtual void Scale(double fac); // as ROOT TProfile::Scale: multiply bin contents (mean of y) and errors by fac
  virtual bool Add(const GmHisto1* his, double fac = 1.); // as ROOT TProfile::Add
  virtual void Sumw2( bool flag = true );

  virtual void GetStats(double* stats) const;
  virtual void PutStats(double* stats);

  virtual void Reset();

protected:
  virtual double RetrieveBinContent(int ibin) const { return (theBinEntries[ibin] > 0) ? theArray[ibin]/theBinEntries[ibin] : 0; }
  virtual void UpdateBinContent(int ibin, double val) { theArray[ibin] = val; } // stored as double, as TH1D
  virtual double GetBinErrorSqUnchecked(int ibin) const { double err = GetBinError(ibin); return err*err; }

protected:
  std::vector<double> theBinEntries; // sum of weights per bin
  std::vector<double> theBinSumw2; // sum of weights squared per bin (empty if not created)
  mutable double theTsumwy;
  mutable double theTsumwy2;

};

GmHistoProfile1 operator+(const GmHistoProfile1 &h1, const GmHistoProfile1 &h2);

#endif

#endif
