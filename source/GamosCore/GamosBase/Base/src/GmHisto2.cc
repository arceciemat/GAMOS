#ifndef GAMOS_ROOT

#include "GmHisto2.hh"
#include <algorithm>
#include <iostream>

// The algorithms reproduce those of ROOT 6.26 classes TH1, TH2, TH2F and TAxis (fixed bin size, no buffer, no axis range, no extendable axes,
// statistics without underflow/overflow (TH1::GetStatOverflows() false), default error option)

//----------------------------------------------------------------------
GmHisto2::GmHisto2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
  : theName(name), theNBinsX(nBinsX), theLowerEdgeX(lowerEdgeX), theUpperEdgeX(upperEdgeX), theNBinsY(nBinsY), theLowerEdgeY(lowerEdgeY), theUpperEdgeY(upperEdgeY)
{
  CheckLimits();
}

//----------------------------------------------------------------------
GmHisto2::GmHisto2( const std::string& name, const std::string& , int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
  : theName(name), theNBinsX(nBinsX), theLowerEdgeX(lowerEdgeX), theUpperEdgeX(upperEdgeX), theNBinsY(nBinsY), theLowerEdgeY(lowerEdgeY), theUpperEdgeY(upperEdgeY)
{
  CheckLimits();
}

//----------------------------------------------------------------------
void GmHisto2::CheckLimits()
{
  //-- Check consistency of data
  if(theNBinsX <= 0) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The number of bins in Xaxis is " << theNBinsX << std::endl;
    std::exception();
    theNBinsX = 1;
  }
  if(theNBinsY <= 0) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The number of bins in Yaxis is " << theNBinsY << std::endl;
    std::exception();
    theNBinsY = 1;
  }

  theBinWidthX = (theUpperEdgeX-theLowerEdgeX)/theNBinsX;
  theBinWidthY = (theUpperEdgeY-theLowerEdgeY)/theNBinsY;

  //-- Check consistency of data
  if(theBinWidthX == 0.) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The lower and upper edges in X axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthX < 0.) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The lower edge in X axis is bigger than the upper edge " << std::endl;
    std::exception();
  }
  if(theBinWidthY == 0.) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The lower and upper edges in Y axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthY < 0.) {
    G4cerr << "!!! ERROR creating GmHisto2: " << theName << "     The lower edge in Y axis is bigger than the upper edge " << std::endl;
    std::exception();
  }

  theNcells = (theNBinsX+2)*(theNBinsY+2);
  theArray.assign(theNcells,0.);
  theSumw2.clear();

  theEntries = 0.;
  theTsumw = 0.;
  theTsumw2 = 0.;
  theTsumwx = 0.;
  theTsumwx2 = 0.;
  theTsumwy = 0.;
  theTsumwy2 = 0.;
  theTsumwxy = 0.;
}

//----------------------------------------------------------------------
int GmHisto2::FindBinX( double value ) const
{
  // as TAxis::FindBin (note the way to catch NaN)
  if( value < theLowerEdgeX ) {
    return 0;
  } else if( !(value < theUpperEdgeX) ) {
    return theNBinsX+1;
  } else {
    return 1 + int( theNBinsX*(value-theLowerEdgeX)/(theUpperEdgeX-theLowerEdgeX) );
  }
}

//----------------------------------------------------------------------
int GmHisto2::FindBinY( double value ) const
{
  // as TAxis::FindBin (note the way to catch NaN)
  if( value < theLowerEdgeY ) {
    return 0;
  } else if( !(value < theUpperEdgeY) ) {
    return theNBinsY+1;
  } else {
    return 1 + int( theNBinsY*(value-theLowerEdgeY)/(theUpperEdgeY-theLowerEdgeY) );
  }
}

//----------------------------------------------------------------------
int GmHisto2::GetBinX(float binx ) const
{
  int ibinX = FindBinX(binx);
  if( ibinX < 1 ){
    G4cerr << "!!! ERROR: GetBinX failed for histogram " << theName << " " << binx << " < " << theLowerEdgeX << std::endl;
  } else if( ibinX > int(theNBinsX) ){
    G4cerr << "!!! ERROR: GetBinX failed for histogram " << theName << " " << binx << " > " << theUpperEdgeX << std::endl;
  }

  return ibinX;
}

//----------------------------------------------------------------------
int GmHisto2::GetBinY(float biny ) const
{
  int ibinY = FindBinY(biny);
  if( ibinY < 1 ){
    G4cerr << "!!! ERROR: GetBinY failed for histogram " << theName << " " << biny << " < " << theLowerEdgeY << std::endl;
  } else if( ibinY > int(theNBinsY) ){
    G4cerr << "!!! ERROR: GetBinY failed for histogram " << theName << " " << biny << " > " << theUpperEdgeY << std::endl;
  }

  return ibinY;
}

//----------------------------------------------------------------------
int GmHisto2::GetBin( int binx, int biny ) const
{
  // as TH2::GetBin
  int ofx = theNBinsX + 1; // overflow bin
  if( binx < 0 ) binx = 0;
  if( binx > ofx ) binx = ofx;
  int ofy = theNBinsY + 1; // overflow bin
  if( biny < 0 ) biny = 0;
  if( biny > ofy ) biny = ofy;
  return binx + (theNBinsX + 2) * biny;
}

//----------------------------------------------------------------------
int GmHisto2::Fill( double valueX, double valueY )
{
  theEntries++;
  int binx = FindBinX(valueX);
  int biny = FindBinY(valueY);
  if( binx < 0 || biny < 0 ) return -1;
  int bin  = biny*(theNBinsX+2) + binx;
  AddBinContent(bin);
  if( theSumw2.size() ) ++theSumw2[bin];
  if( binx == 0 || binx > theNBinsX ) return -1;
  if( biny == 0 || biny > theNBinsY ) return -1;
  ++theTsumw;
  ++theTsumw2;
  theTsumwx  += valueX;
  theTsumwx2 += valueX*valueX;
  theTsumwy  += valueY;
  theTsumwy2 += valueY*valueY;
  theTsumwxy += valueX*valueY;
  return bin;
}

//----------------------------------------------------------------------
int GmHisto2::Fill( double valueX, double valueY, double weight )
{
  theEntries++;
  int binx = FindBinX(valueX);
  int biny = FindBinY(valueY);
  if( binx < 0 || biny < 0 ) return -1;
  int bin  = biny*(theNBinsX+2) + binx;
  if( !theSumw2.size() && weight != 1.0 ) Sumw2(); // must be called before AddBinContent
  if( theSumw2.size() ) theSumw2[bin] += weight*weight;
  AddBinContent(bin, weight);
  if( binx == 0 || binx > theNBinsX ) return -1;
  if( biny == 0 || biny > theNBinsY ) return -1;
  double z = weight;
  theTsumw   += z;
  theTsumw2  += z*z;
  theTsumwx  += z*valueX;
  theTsumwx2 += z*valueX*valueX;
  theTsumwy  += z*valueY;
  theTsumwy2 += z*valueY*valueY;
  theTsumwxy += z*valueX*valueY;
  return bin;
}

//----------------------------------------------------------------------
void GmHisto2::AddBinContent(int ibin)
{
  float val = float(theArray[ibin]);
  ++val;
  theArray[ibin] = double(val);
}

//----------------------------------------------------------------------
void GmHisto2::AddBinContent(int ibin, double val)
{
  float valf = float(theArray[ibin]);
  valf += float(val);
  theArray[ibin] = double(valf);
}

//----------------------------------------------------------------------
double GmHisto2::GetBinContent(int ibin) const
{
  if( ibin < 0 ) ibin = 0;
  if( ibin >= theNcells ) ibin = theNcells-1;
  return RetrieveBinContent(ibin);
}

//----------------------------------------------------------------------
double GmHisto2::GetBinContent(int ibinX, int ibinY) const
{
  return GmHisto2::GetBinContent( GetBin(ibinX,ibinY) );
}

//----------------------------------------------------------------------
double GmHisto2::GetBinError(int ibin) const
{
  if( ibin < 0 ) ibin = 0;
  if( ibin >= theNcells ) ibin = theNcells-1;
  if( theSumw2.size() ) return sqrt(theSumw2[ibin]);
  return sqrt(fabs(RetrieveBinContent(ibin)));
}

//----------------------------------------------------------------------
double GmHisto2::GetBinError(int ibinX, int ibinY) const
{
  return GetBinError( GetBin(ibinX,ibinY) );
}

//----------------------------------------------------------------------
double GmHisto2::GetBinSumW(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0.;
  return theArray[ibin];
}

//----------------------------------------------------------------------
double GmHisto2::GetBinSumW2(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0.;
  return GetBinErrorSqUnchecked(ibin);
}

//----------------------------------------------------------------------
void GmHisto2::SetBinContent(int ibin, double val)
{
  theEntries++;
  theTsumw = 0; // statistics will be recalculated from bin contents
  if( ibin < 0 ) return;
  if( ibin >= theNcells ) return;
  UpdateBinContent(ibin, val);
}

//----------------------------------------------------------------------
void GmHisto2::SetBinContent(int ibinX, int ibinY, double val)
{
  SetBinContent( GetBin(ibinX,ibinY), val );
}

//----------------------------------------------------------------------
void GmHisto2::SetBinError(int ibin, double val)
{
  if( ibin < 0 || ibin >= theNcells ) return;
  if( !theSumw2.size() ) Sumw2();
  theSumw2[ibin] = val*val;
}

//----------------------------------------------------------------------
void GmHisto2::SetBinError(int ibinX, int ibinY, double val)
{
  if( ibinX < 0 || ibinX > theNBinsX + 1 ) return;
  if( ibinY < 0 || ibinY > theNBinsY + 1 ) return;
  SetBinError( GetBin(ibinX,ibinY), val );
}

//----------------------------------------------------------------------
void GmHisto2::Sumw2( bool flag )
{
  if( !flag ) {
    theSumw2.clear();
    return;
  }

  if( int(theSumw2.size()) == theNcells ) {
    G4cerr << "Warning in <GmHisto2::Sumw2>: Sum of squares of weights structure already created " << theName << std::endl;
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
void GmHisto2::GetStats(double* stats) const
{
  if( theTsumw == 0 && theEntries > 0 ) {
    // recalculate statistics from bin contents
    std::fill(stats, stats + 7, 0);
    for( int biny = 1; biny <= theNBinsY; ++biny ) {
      double y = GetBinCenterY(biny);
      for( int binx = 1; binx <= theNBinsX; ++binx ) {
	double x = GetBinCenterX(binx);
	int bin = GetBin(binx,biny);
	double w = RetrieveBinContent(bin);
	double wx = w * x; // avoid some extra multiplications at the expense of some clarity
	double wy = w * y;
	stats[0] += w;
	stats[1] += GetBinErrorSqUnchecked(bin);
	stats[2] += wx;
	stats[3] += wx * x;
	stats[4] += wy;
	stats[5] += wy * y;
	stats[6] += wx * y;
      }
    }
  } else {
    stats[0] = theTsumw;
    stats[1] = theTsumw2;
    stats[2] = theTsumwx;
    stats[3] = theTsumwx2;
    stats[4] = theTsumwy;
    stats[5] = theTsumwy2;
    stats[6] = theTsumwxy;
  }
}

//----------------------------------------------------------------------
void GmHisto2::PutStats(double* stats)
{
  theTsumw   = stats[0];
  theTsumw2  = stats[1];
  theTsumwx  = stats[2];
  theTsumwx2 = stats[3];
  theTsumwy  = stats[4];
  theTsumwy2 = stats[5];
  theTsumwxy = stats[6];
}

//----------------------------------------------------------------------
void GmHisto2::ResetStats()
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
double GmHisto2::GetMean( int axis ) const
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
double GmHisto2::GetStdDev( int axis ) const
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
    // formula valid for only gaussian distribution ( 4-th momentum =  3 * sigma^4 )
    double neff = GetEffectiveEntries();
    return ( neff > 0 ? sqrt(stddev2/(2*neff) ) : 0. );
  }
}

//----------------------------------------------------------------------
double GmHisto2::GetEffectiveEntries() const
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
double GmHisto2::Integral() const
{
  double integral = 0.;
  for( int binx = 1; binx <= theNBinsX; binx++ ) {
    for( int biny = 1; biny <= theNBinsY; biny++ ) {
      integral += RetrieveBinContent(GetBin(binx,biny));
    }
  }
  return integral;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinLowEdgeX( int ibin ) const
{
  double binwidth = (theUpperEdgeX - theLowerEdgeX) / double(theNBinsX);
  return theLowerEdgeX + (ibin-1) * binwidth;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinUpEdgeX( int ibin ) const
{
  double binwidth = (theUpperEdgeX - theLowerEdgeX) / double(theNBinsX);
  return theLowerEdgeX + ibin*binwidth;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinLowEdgeY( int ibin ) const
{
  double binwidth = (theUpperEdgeY - theLowerEdgeY) / double(theNBinsY);
  return theLowerEdgeY + (ibin-1) * binwidth;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinUpEdgeY( int ibin ) const
{
  double binwidth = (theUpperEdgeY - theLowerEdgeY) / double(theNBinsY);
  return theLowerEdgeY + ibin*binwidth;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinCenterX( int ibin ) const
{
  double binwidth = (theUpperEdgeX - theLowerEdgeX) / double(theNBinsX);
  return theLowerEdgeX + (ibin-1) * binwidth + 0.5*binwidth;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinCenterY( int ibin ) const
{
  double binwidth = (theUpperEdgeY - theLowerEdgeY) / double(theNBinsY);
  return theLowerEdgeY + (ibin-1) * binwidth + 0.5*binwidth;
}

//----------------------------------------------------------------------
void GmHisto2::Scale(double fac)
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
bool GmHisto2::Add(const GmHisto2* his, double fac)
{
  if( !his ) {
    G4cerr << "Error in <GmHisto2::Add>: Attempt to add a non-existing histogram" << std::endl;
    return false;
  }
  if( his->GetNbinsX() != theNBinsX || his->GetNbinsY() != theNBinsY ) {
    G4cerr << "Error in <GmHisto2::Add>: Attempt to add histograms with different number of bins" << std::endl;
    return false;
  }
  if( his->GetLowerEdgeX() != theLowerEdgeX || his->GetUpperEdgeX() != theUpperEdgeX
      || his->GetLowerEdgeY() != theLowerEdgeY || his->GetUpperEdgeY() != theUpperEdgeY ) {
    G4cerr << "Warning in <GmHisto2::Add>: Attempt to add histograms with different axis limits" << std::endl;
  }

  //   Create Sumw2 if his has Sumw2 set
  if( theSumw2.size() == 0 && his->GetSumw2N() != 0 ) Sumw2();

  //   - Add statistics
  double entries = fabs( GetEntries() + fac * his->GetEntries() );

  // statistics can be preserved only in case of positive coefficients
  bool resetStats = (fac < 0);
  double s1[kNstat] = {0};
  double s2[kNstat] = {0};
  if( !resetStats ) {
    GetStats(s1);
    his->GetStats(s2);
  }

  double fac2 = fac * fac;
  for( int bin = 0; bin < theNcells; ++bin ) {
    AddBinContent(bin, fac * his->RetrieveBinContent(bin));
    if( theSumw2.size() ) theSumw2[bin] += fac2 * his->GetBinErrorSqUnchecked(bin);
  }

  if( resetStats )  {
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
GmHisto2 operator+(const GmHisto2 &h1, const GmHisto2 &h2)
{
  GmHisto2 hnew = h1;
  hnew.Add(&h2,1);
  return hnew;
}

//----------------------------------------------------------------------
void GmHisto2::Reset()
{
  std::fill(theArray.begin(), theArray.end(), 0.);
  std::fill(theSumw2.begin(), theSumw2.end(), 0.);
  theTsumw = 0;
  theTsumw2 = 0;
  theTsumwx = 0;
  theTsumwx2 = 0;
  theTsumwy = 0;
  theTsumwy2 = 0;
  theTsumwxy = 0;
  theEntries = 0;
}

#endif
