#ifdef GAMOS_NO_ROOT

#include "GmHisto1.hh"
#include <algorithm>
#include <iostream>

// The algorithms reproduce those of ROOT 6.26 classes TH1, TH1F and TAxis (fixed bin size, no buffer, no axis range, no extendable axes,
// statistics without underflow/overflow (TH1::GetStatOverflows() false), default error option)

//----------------------------------------------------------------------
GmHisto1::GmHisto1( const std::string& name, int nBins, double lowerEdge, double upperEdge )
  : theName(name), theNBins(nBins), theLowerEdge(lowerEdge), theUpperEdge(upperEdge)
{
  CheckLimits();
}

//----------------------------------------------------------------------
GmHisto1::GmHisto1( const std::string& name, const std::string& , int nBins, double lowerEdge, double upperEdge )
  : theName(name), theNBins(nBins), theLowerEdge(lowerEdge), theUpperEdge(upperEdge)
{
  CheckLimits();
}

//----------------------------------------------------------------------
void GmHisto1::CheckLimits()
{
  //-- Check consistency of data
  if(theNBins <= 0) {
    G4cerr << "!!! ERROR creating GmHisto1: " << theName << "     The number of bins is " << theNBins << std::endl;
    std::exception();
    theNBins = 1;
  }

  theBinWidth = (theUpperEdge-theLowerEdge)/theNBins;

  //-- Check consistency of data
  if(theBinWidth == 0.) {
    G4cerr << "!!! ERROR creating GmHisto1: " << theName << "     The lower and upper edges are equal " << std::endl;
    std::exception();
  }
  if(theBinWidth < 0.) {
    G4cerr << "!!! ERROR creating GmHisto1: " << theName << "     The lower edge is bigger than the upper edge " << std::endl;
    std::exception();
  }

  theNcells = theNBins+2;
  theArray.assign(theNcells,0.);
  theSumw2.clear();

  theEntries = 0.;
  theTsumw = 0.;
  theTsumw2 = 0.;
  theTsumwx = 0.;
  theTsumwx2 = 0.;
  theMaximum = -1111;
  theMinimum = -1111;
}

//----------------------------------------------------------------------
int GmHisto1::FindBin( double value ) const
{
  // as TAxis::FindBin (note the way to catch NaN)
  if( value < theLowerEdge ) {
    return 0;
  } else if( !(value < theUpperEdge) ) {
    return theNBins+1;
  } else {
    return 1 + int( theNBins*(value-theLowerEdge)/(theUpperEdge-theLowerEdge) );
  }
}

//----------------------------------------------------------------------
int GmHisto1::GetBin(double binx ) const
{
  int ibin = FindBin(binx);
  if( ibin < 1 ){
    G4cerr << "!!! ERROR: GetBin failed for histogram " << theName << " " << binx << " < " << theLowerEdge << std::endl;
  } else if( ibin > int(theNBins) ){
    G4cerr << "!!! ERROR: GetBin failed for histogram " << theName << " " << binx << " > " << theUpperEdge << std::endl;
  }

  return ibin;
}

//----------------------------------------------------------------------
int GmHisto1::Fill( double value )
{
  theEntries++;
  int bin = FindBin(value);
  if( bin < 0 ) return -1;
  AddBinContent(bin);
  if( theSumw2.size() ) ++theSumw2[bin];
  if( bin == 0 || bin > theNBins ) return -1;
  ++theTsumw;
  ++theTsumw2;
  theTsumwx  += value;
  theTsumwx2 += value*value;
  return bin;
}

//----------------------------------------------------------------------
int GmHisto1::Fill( double value, double weight )
{
  theEntries++;
  int bin = FindBin(value);
  if( bin < 0 ) return -1;
  if( !theSumw2.size() && weight != 1.0 ) Sumw2(); // must be called before AddBinContent
  if( theSumw2.size() ) theSumw2[bin] += weight*weight;
  AddBinContent(bin, weight);
  if( bin == 0 || bin > theNBins ) return -1;
  double z = weight;
  theTsumw   += z;
  theTsumw2  += z*z;
  theTsumwx  += z*value;
  theTsumwx2 += z*value*value;
  return bin;
}

//----------------------------------------------------------------------
void GmHisto1::AddBinContent(int ibin)
{
  float val = float(theArray[ibin]);
  ++val;
  theArray[ibin] = double(val);
}

//----------------------------------------------------------------------
void GmHisto1::AddBinContent(int ibin, double val)
{
  float valf = float(theArray[ibin]);
  valf += float(val);
  theArray[ibin] = double(valf);
}

//----------------------------------------------------------------------
double GmHisto1::GetBinContent(int ibin) const
{
  if( ibin < 0 ) ibin = 0;
  if( ibin >= theNcells ) ibin = theNcells-1;
  return RetrieveBinContent(ibin);
}

//----------------------------------------------------------------------
double GmHisto1::GetBinError(int ibin) const
{
  if( ibin < 0 ) ibin = 0;
  if( ibin >= theNcells ) ibin = theNcells-1;
  if( theSumw2.size() ) return sqrt(theSumw2[ibin]);
  return sqrt(fabs(RetrieveBinContent(ibin)));
}

//----------------------------------------------------------------------
double GmHisto1::GetBinSumW(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0.;
  return theArray[ibin];
}

//----------------------------------------------------------------------
double GmHisto1::GetBinSumW2(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0.;
  return GetBinErrorSqUnchecked(ibin);
}

//----------------------------------------------------------------------
void GmHisto1::SetBinContent(int ibin, double val)
{
  theEntries++;
  theTsumw = 0; // statistics will be recalculated from bin contents
  if( ibin < 0 ) return;
  if( ibin >= theNcells-1 ) {
    if( ibin == theNcells-1 ) UpdateBinContent(ibin, val);
    return;
  }
  UpdateBinContent(ibin, val);
}

//----------------------------------------------------------------------
void GmHisto1::SetBinError(int ibin, double val)
{
  if( ibin < 0 || ibin >= theNcells ) return;
  if( !theSumw2.size() ) Sumw2();
  theSumw2[ibin] = val*val;
}

//----------------------------------------------------------------------
void GmHisto1::Sumw2( bool flag )
{
  if( !flag ) {
    theSumw2.clear();
    return;
  }

  if( int(theSumw2.size()) == theNcells ) {
    G4cerr << "Warning in <GmHisto1::Sumw2>: Sum of squares of weights structure already created " << theName << std::endl;
    return;
  }

  theSumw2.assign(theNcells,0.);
  if( theEntries > 0 ) {
    for( int ii = 0; ii < theNcells; ii++ ) {
      theSumw2[ii] = fabs(RetrieveBinContent(ii));
    }
  }
}

//----------------------------------------------------------------------
void GmHisto1::GetStats(double* stats) const
{
  if( theTsumw == 0 && theEntries > 0 ) {
    // recalculate statistics from bin contents
    for( int ii = 0; ii < 4; ii++ ) stats[ii] = 0;
    for( int binx = 1; binx <= theNBins; binx++ ) {
      double x = GetBinCenter(binx);
      double w = RetrieveBinContent(binx);
      double err = fabs(GetBinError(binx));
      stats[0] += w;
      stats[1] += err*err;
      stats[2] += w*x;
      stats[3] += w*x*x;
    }
  } else {
    stats[0] = theTsumw;
    stats[1] = theTsumw2;
    stats[2] = theTsumwx;
    stats[3] = theTsumwx2;
  }
}

//----------------------------------------------------------------------
void GmHisto1::PutStats(double* stats)
{
  theTsumw   = stats[0];
  theTsumw2  = stats[1];
  theTsumwx  = stats[2];
  theTsumwx2 = stats[3];
}

//----------------------------------------------------------------------
void GmHisto1::ResetStats()
{
  double stats[kNstat] = {0};
  theTsumw = 0;
  theEntries = 1; // to force re-calculation of the statistics in GetStats
  GetStats(stats);
  PutStats(stats);
  theEntries = fabs(theTsumw);
  // use effective entries for weighted histograms:  (sum_w) ^2 / sum_w2
  if( theSumw2.size() && theTsumw > 0 && stats[1] > 0 ) theEntries = stats[0]*stats[0]/ stats[1];
}

//----------------------------------------------------------------------
double GmHisto1::GetMean(int axis) const
{
  if( axis < 1 || (axis > 3 && axis < 11) || axis > 13 ) return 0;
  double stats[kNstat] = {0};
  GetStats(stats);
  if( stats[0] == 0 ) return 0;
  if( axis < 4 ) {
    int ax[3] = {2,4,7};
    return stats[ax[axis-1]]/stats[0];
  } else {
    // mean error = std dev / sqrt( Neff )
    double stddev = GetStdDev(axis-10);
    double neff = GetEffectiveEntries();
    return ( neff > 0 ? stddev/sqrt(neff) : 0. );
  }
}

//----------------------------------------------------------------------
double GmHisto1::GetStdDev(int axis) const
{
  if( axis < 1 || (axis > 3 && axis < 11) || axis > 13 ) return 0;
  double stats[kNstat] = {0};
  GetStats(stats);
  if( stats[0] == 0 ) return 0;
  int ax[3] = {2,4,7};
  int axm = ax[axis%10 - 1];
  double x = stats[axm]/stats[0];
  // for negative stddev (e.g. when having negative weights) - return stdev=0
  double stddev2 = std::max( stats[axm+1]/stats[0] -x*x, 0.0 );
  if( axis < 10 ) {
    return sqrt(stddev2);
  } else {
    // The right formula for StdDev error depends on 4th momentum (see Kendall-Stuart Vol 1 pag 243)
    // formula valid for only gaussian distribution ( 4-th momentum =  3 * sigma^4 )
    double neff = GetEffectiveEntries();
    return ( neff > 0 ? sqrt(stddev2/(2*neff) ) : 0. );
  }
}

//----------------------------------------------------------------------
double GmHisto1::GetEffectiveEntries() const
{
  // number of effective entries of the histogram,
  // i.e. the number of unweighted entries a histogram would need to
  // have the same statistical power as this histogram with possibly
  // weighted entries
  double stats[kNstat] = {0};
  GetStats(stats); // s[1] sum of squares of weights, s[0] sum of weights
  return ( stats[1] ? stats[0]*stats[0]/stats[1] : fabs(stats[0]) );
}

//----------------------------------------------------------------------
double GmHisto1::GetMaximum( double maxval ) const
{
  if( theMaximum != -1111 ) return theMaximum;
  double maximum = -FLT_MAX;
  for( int ii = 1; ii <= theNBins; ii++ ) {
    double val = GetBinContent(ii);
    if( val > maximum && val < maxval ) maximum = val;
  }
  return maximum;
}

//----------------------------------------------------------------------
double GmHisto1::GetMinimum( double minval ) const
{
  if( theMinimum != -1111 ) return theMinimum;
  double minimum = FLT_MAX;
  for( int ii = 1; ii <= theNBins; ii++ ) {
    double val = GetBinContent(ii);
    if( val < minimum && val > minval ) minimum = val;
  }
  return minimum;
}

//----------------------------------------------------------------------
int GmHisto1::GetMaximumBin() const
{
  int ibinMax = 1;
  for( int ii = 2; ii <= theNBins; ii++ ) {
    if( GetBinContent(ii) > GetBinContent(ibinMax) ) ibinMax = ii;
  }
  return ibinMax;
}

//----------------------------------------------------------------------
int GmHisto1::GetMinimumBin() const
{
  int ibinMin = 1;
  for( int ii = 2; ii <= theNBins; ii++ ) {
    if( GetBinContent(ii) < GetBinContent(ibinMin) ) ibinMin = ii;
  }
  return ibinMin;
}

//----------------------------------------------------------------------
double GmHisto1::Integral() const
{
  double integral = 0.;
  for( int ii = 1; ii <= theNBins; ii++ ) {
    integral += RetrieveBinContent(ii);
  }
  return integral;
}

//----------------------------------------------------------------------
double GmHisto1::GetBinLowEdge( int ibin ) const
{
  double binwidth = (theUpperEdge - theLowerEdge) / double(theNBins);
  return theLowerEdge + (ibin-1) * binwidth;
}

//----------------------------------------------------------------------
double GmHisto1::GetBinUpEdge( int ibin ) const
{
  double binwidth = (theUpperEdge - theLowerEdge) / double(theNBins);
  return theLowerEdge + ibin*binwidth;
}

//----------------------------------------------------------------------
double GmHisto1::GetBinCenter( int ibin ) const
{
  double binwidth = (theUpperEdge - theLowerEdge) / double(theNBins);
  return theLowerEdge + (ibin-1) * binwidth + 0.5*binwidth;
}

//----------------------------------------------------------------------
void GmHisto1::Scale(double fac)
{
  // store bin errors when scaling since cannot anymore be computed as sqrt(N)
  if( GetSumw2N() == 0 ) Sumw2();
  for( int ii = 0; ii < theNcells; ++ii ) UpdateBinContent(ii, fac * RetrieveBinContent(ii));
  if( theSumw2.size() ) for( int ii = 0; ii < theNcells; ++ii ) theSumw2[ii] *= (fac * fac); // update errors
  // update global histograms statistics
  double stats[kNstat] = {0};
  GetStats(stats);
  for( int ii = 0; ii < kNstat; ii++ ) {
    if( ii == 1 ) stats[ii] = fac*fac*stats[ii];
    else          stats[ii] = fac*stats[ii];
  }
  PutStats(stats);
}

//----------------------------------------------------------------------
bool GmHisto1::Add(const GmHisto1* his, double fac)
{
  if( !his ) {
    G4cerr << "Error in <GmHisto1::Add>: Attempt to add a non-existing histogram" << std::endl;
    return false;
  }
  if( his->GetNbins() != theNBins ) {
    G4cerr << "Error in <GmHisto1::Add>: Attempt to add histograms with different number of bins : nbins h1 = " << theNBins << " , nbins h2 =  " << his->GetNbins() << std::endl;
    return false;
  }
  if( his->GetLowerEdge() != theLowerEdge || his->GetUpperEdge() != theUpperEdge ) {
    G4cerr << "Warning in <GmHisto1::Add>: Attempt to add histograms with different axis limits" << std::endl;
  }

  //   Create Sumw2 if his has Sumw2 set
  if( theSumw2.size() == 0 && his->GetSumw2N() != 0 ) Sumw2();

  //   - Add statistics
  double entries = fabs( GetEntries() + fac * his->GetEntries() );

  // statistics can be preserved only in case of positive coefficients
  // otherwise with negative fac (histogram subtraction) one risks to get negative variances
  bool resetStats = (fac < 0);
  double s1[kNstat] = {0};
  double s2[kNstat] = {0};
  if( !resetStats ) {
    // need to initialize to zero s1 and s2 since
    // GetStats fills only used elements depending on dimension and type
    GetStats(s1);
    his->GetStats(s2);
  }

  double fac2 = fac * fac;
  for( int bin = 0; bin < theNcells; ++bin ) {
    AddBinContent(bin, fac * his->RetrieveBinContent(bin));
    if( theSumw2.size() ) theSumw2[bin] += fac2 * his->GetBinErrorSqUnchecked(bin);
  }

  // update statistics (do here to avoid changes by SetBinContent)
  if( resetStats )  {
    // statistics need to be reset in case coefficient are negative
    ResetStats();
  } else {
    for( int ii = 0; ii < kNstat; ii++ ) {
      if( ii == 1 ) s1[ii] += fac*fac*s2[ii];
      else          s1[ii] += fac*s2[ii];
    }
    PutStats(s1);
    SetEntries(entries);
  }
  return true;
}

//----------------------------------------------------------------------
GmHisto1 operator+(const GmHisto1 &h1, const GmHisto1 &h2)
{
  GmHisto1 hnew = h1;
  hnew.Add(&h2,1);
  return hnew;
}

//----------------------------------------------------------------------
void GmHisto1::Reset()
{
  std::fill(theArray.begin(), theArray.end(), 0.);
  std::fill(theSumw2.begin(), theSumw2.end(), 0.);
  theTsumw = 0;
  theTsumw2 = 0;
  theTsumwx = 0;
  theTsumwx2 = 0;
  theEntries = 0;
}

#endif
