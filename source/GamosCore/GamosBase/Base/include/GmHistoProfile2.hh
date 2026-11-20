#ifndef GmHistoProfile2_hh
#define GmHistoProfile2_hh
#ifdef GAMOS_NO_ROOT

#include "GmHisto2.hh"
#include <iostream>

//------------------------------------------------------------------------
// 2D profile histogram used when GAMOS is compiled without ROOT (GAMOS_NO_ROOT).
// It reproduces the behaviour of ROOT TProfile2D (ROOT 6.26) with default error option (error on the mean), that is used when GAMOS is compiled with ROOT:
//  for each bin it stores sum(w*z) (in theArray), sum(w*z*z) (in theSumw2), sum(w) (in theBinEntries) and sum(w*w) (in theBinSumw2, only once a weight != 1 is used)
//  bin content is the mean of z: sum(w*z)/sum(w)
//------------------------------------------------------------------------
class GmHistoProfile2 : public GmHisto2
{
public:
  GmHistoProfile2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHistoProfile2( const std::string& name,const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHistoProfile2( const GmHistoProfile2& his ) = default;
  GmHistoProfile2& operator=( const GmHistoProfile2& his ) = default;
  virtual ~GmHistoProfile2(){};

  virtual int Fill( double, double ) { G4cerr << "Error in <GmHistoProfile2::Fill(double,double)>: may not use (as TProfile2D)" << std::endl; return -1; }
  virtual int Fill( double valueX, double valueY, double valueZ );
  virtual int Fill( double valueX, double valueY, double valueZ, double weight );

  virtual double GetBinContent(int ibinX, int ibinY) const;
  virtual double GetBinError(int ibinX, int ibinY) const;
  double GetBinEntries(int ibin) const; // global bin number
  double GetBinEffectiveEntries(int ibin) const; // global bin number
  virtual void AddBinContent(int ibin);
  virtual void AddBinContent(int ibin, double val);
  virtual void Scale(double fac); // as ROOT TProfile2D::Scale: multiply bin contents (mean of z) and errors by fac
  virtual bool Add(const GmHisto2* his, double fac = 1.); // as ROOT TProfile2D::Add
  virtual void Sumw2( bool flag = true );

  virtual void GetStats(double* stats) const;
  virtual void PutStats(double* stats);

  virtual void Reset();

protected:
  virtual double GetBinContent(int ibin) const;
  virtual double GetBinError(int ibin) const;
  virtual double RetrieveBinContent(int ibin) const { return (theBinEntries[ibin] > 0) ? theArray[ibin]/theBinEntries[ibin] : 0; }
  virtual void UpdateBinContent(int ibin, double val) { theArray[ibin] = val; } // stored as double, as TH2D
  virtual double GetBinErrorSqUnchecked(int ibin) const { double err = GetBinError(ibin); return err*err; }

protected:
  std::vector<double> theBinEntries; // sum of weights per bin
  std::vector<double> theBinSumw2; // sum of weights squared per bin (empty if not created)
  double theTsumwz;
  double theTsumwz2;

};

GmHistoProfile2 operator+(const GmHistoProfile2 &h1, const GmHistoProfile2 &h2);

#endif

#endif
