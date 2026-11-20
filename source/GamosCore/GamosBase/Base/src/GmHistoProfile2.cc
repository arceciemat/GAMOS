#ifdef GAMOS_NO_ROOT

#include "GmHistoProfile2.hh"
#include <algorithm>

// The algorithms reproduce those of ROOT 6.26 classes TProfile2D and TProfileHelper (default error option: error on the mean,
// no z limits, no buffer, statistics without underflow/overflow)

//----------------------------------------------------------------------
GmHistoProfile2::GmHistoProfile2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
  : GmHisto2( name, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY )
{
  theBinEntries.assign(theNcells,0.);
  theSumw2.assign(theNcells,0.);
  theBinSumw2.clear();
  theTsumwz = theTsumwz2 = 0;
}

//----------------------------------------------------------------------
GmHistoProfile2::GmHistoProfile2( const std::string& name, const std::string&, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
  : GmHisto2( name, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY )
{
  theBinEntries.assign(theNcells,0.);
  theSumw2.assign(theNcells,0.);
  theBinSumw2.clear();
  theTsumwz = theTsumwz2 = 0;
}

//----------------------------------------------------------------------
int GmHistoProfile2::Fill( double valueX, double valueY, double valueZ )
{
  theEntries++;
  int binx = FindBinX(valueX);
  int biny = FindBinY(valueY);
  if( binx < 0 || biny < 0 ) return -1;
  int bin = GetBin(binx, biny);
  theArray[bin] += valueZ;
  theSumw2[bin] += valueZ*valueZ;
  theBinEntries[bin] += 1;
  if( theBinSumw2.size() ) theBinSumw2[bin] += 1;
  if( binx == 0 || binx > theNBinsX ) return -1;
  if( biny == 0 || biny > theNBinsY ) return -1;
  ++theTsumw;
  ++theTsumw2;
  theTsumwx  += valueX;
  theTsumwx2 += valueX*valueX;
  theTsumwy  += valueY;
  theTsumwy2 += valueY*valueY;
  theTsumwxy += valueX*valueY;
  theTsumwz  += valueZ;
  theTsumwz2 += valueZ*valueZ;
  return bin;
}

//----------------------------------------------------------------------
int GmHistoProfile2::Fill( double valueX, double valueY, double valueZ, double weight )
{
  double u = weight;
  theEntries++;
  int binx = FindBinX(valueX);
  int biny = FindBinY(valueY);
  if( binx < 0 || biny < 0 ) return -1;
  int bin = biny * (theNBinsX + 2) + binx;
  AddBinContent(bin, u * valueZ);
  theSumw2[bin] += u * valueZ * valueZ;
  if( !theBinSumw2.size() && u != 1.0 ) Sumw2(); // must be called before accumulating the entries
  if( theBinSumw2.size() ) theBinSumw2[bin] += u * u;
  theBinEntries[bin] += u;
  if( binx == 0 || binx > theNBinsX ) return -1;
  if( biny == 0 || biny > theNBinsY ) return -1;
  theTsumw   += u;
  theTsumw2  += u * u;
  theTsumwx  += u * valueX;
  theTsumwx2 += u * valueX * valueX;
  theTsumwy  += u * valueY;
  theTsumwy2 += u * valueY * valueY;
  theTsumwxy += u * valueX * valueY;
  theTsumwz  += u * valueZ;
  theTsumwz2 += u * valueZ * valueZ;
  return bin;
}

//----------------------------------------------------------------------
void GmHistoProfile2::AddBinContent(int ibin)
{
  ++theArray[ibin];
}

//----------------------------------------------------------------------
void GmHistoProfile2::AddBinContent(int ibin, double val)
{
  theArray[ibin] += val;
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinContent(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  if( theBinEntries[ibin] == 0 ) return 0;
  return theArray[ibin]/theBinEntries[ibin];
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinContent(int ibinX, int ibinY) const
{
  return GetBinContent( GetBin(ibinX,ibinY) );
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinEntries(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  return theBinEntries[ibin];
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinEffectiveEntries(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  double sumOfWeights = theBinEntries[ibin];
  if( theBinSumw2.size() == 0 || int(theBinSumw2.size()) != theNcells ) {
    // this can happen  when reading an old file
    return sumOfWeights;
  }
  double sumOfWeightsSquare = theBinSumw2[ibin];
  return ( sumOfWeightsSquare > 0 ?  sumOfWeights * sumOfWeights /   sumOfWeightsSquare : 0 );
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinError(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  double cont = theArray[ibin];        // sum of bin w *z
  double sum  = theBinEntries[ibin];   // sum of bin weights
  double err2 = theSumw2[ibin];        // sum of bin w * z^2
  double neff = GetBinEffectiveEntries(ibin);  // (sum of w)^2 / (sum of w^2)
  if( sum == 0 ) return 0;      // for empty bins
  double contsum = cont/sum;
  double eprim2  = fabs(err2/sum - contsum*contsum);
  double eprim   = sqrt(eprim2);
  // return standard error on the mean of z
  return eprim/sqrt(neff);
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinError(int ibinX, int ibinY) const
{
  return GetBinError( GetBin(ibinX,ibinY) );
}

//----------------------------------------------------------------------
void GmHistoProfile2::Sumw2( bool flag )
{
  if( !flag ) {
    theBinSumw2.clear();
    return;
  }

  if( int(theBinSumw2.size()) == theNcells ) {
    G4cerr << "Warning in <GmHistoProfile2::Sumw2>: Sum of squares of profile bin weights structure already created " << theName << std::endl;
    return;
  }

  theBinSumw2 = theBinEntries;
}

//----------------------------------------------------------------------
void GmHistoProfile2::GetStats(double* stats) const
{
  if( theTsumw == 0 ) {
    // recalculate statistics from bin contents
    for( int ii = 0; ii < 9; ii++ ) stats[ii] = 0;
    for( int biny = 1; biny <= theNBinsY; biny++ ) {
      double y = GetBinCenterY(biny);
      for( int binx = 1; binx <= theNBinsX; binx++ ) {
	int bin = GetBin(binx,biny);
	double w  = theBinEntries[bin];
	double w2 = (theBinSumw2.size() ? theBinSumw2[bin] : w );
	double x  = GetBinCenterX(binx);
	stats[0] += w;
	stats[1] += w2;
	stats[2] += w*x;
	stats[3] += w*x*x;
	stats[4] += w*y;
	stats[5] += w*y*y;
	stats[6] += w*x*y;
	stats[7] += theArray[bin];
	stats[8] += theSumw2[bin];
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
    stats[7] = theTsumwz;
    stats[8] = theTsumwz2;
  }
}

//----------------------------------------------------------------------
void GmHistoProfile2::PutStats(double* stats)
{
  theTsumw   = stats[0];
  theTsumw2  = stats[1];
  theTsumwx  = stats[2];
  theTsumwx2 = stats[3];
  theTsumwy  = stats[4];
  theTsumwy2 = stats[5];
  theTsumwxy = stats[6];
  theTsumwz  = stats[7];
  theTsumwz2 = stats[8];
}

//----------------------------------------------------------------------
void GmHistoProfile2::Scale(double fac)
{
  // as TProfileHelper::Scale: statistics are not changed
  double afac = fabs(fac);
  for( int bin = 0; bin < theNcells; bin++ ) {
    theArray[bin] = fac*theArray[bin];
    theSumw2[bin] = afac*afac*theSumw2[bin];
  }
}

//----------------------------------------------------------------------
bool GmHistoProfile2::Add(const GmHisto2* his2, double fac)
{
  // as TProfile2D::Add(h1,c1), that calls TProfileHelper::Add(this, this, h1, 1, c1)
  const GmHistoProfile2* his = dynamic_cast<const GmHistoProfile2*>(his2);
  if( !his ) {
    G4cerr << "Error in <GmHistoProfile2::Add>: Attempt to add a non-existing profile or a non-profile object" << std::endl;
    return false;
  }
  if( his->GetNbinsX() != theNBinsX || his->GetNbinsY() != theNBinsY ) {
    G4cerr << "Error in <GmHistoProfile2::Add>: Attempt to add profiles with different number of bins" << std::endl;
    return false;
  }

  double c1 = 1.;
  double c2 = fac;
  double ac1 = fabs(c1);
  double ac2 = fabs(c2);
  theEntries = ac1*GetEntries() + ac2*his->GetEntries();
  double s0[kNstat] = {0}, s1[kNstat] = {0}, s2[kNstat] = {0};
  GetStats(s0);
  GetStats(s1);
  his->GetStats(s2);
  for( int ii = 0; ii < kNstat; ii++ ) {
    if( ii == 1 ) s0[ii] = c1*c1*s1[ii] + c2*c2*s2[ii];
    else          s0[ii] = ac1*s1[ii] + ac2*s2[ii];
  }
  PutStats(s0);

  // create sumw2 per bin if not set
  if( theBinSumw2.size() == 0 && his->theBinSumw2.size() != 0 ) Sumw2();

  // Make the loop over the bins to calculate the Addition
  const std::vector<double>& ew1 = (theBinSumw2.size() ? theBinSumw2 : theBinEntries);
  const std::vector<double>& ew2 = (his->theBinSumw2.size() ? his->theBinSumw2 : his->theBinEntries);
  for( int bin = 0; bin < theNcells; bin++ ) {
    double ew1b = ew1[bin]; // copy before modifying theBinEntries
    theArray[bin]      = c1*theArray[bin] + c2*his->theArray[bin];
    theSumw2[bin]      = ac1*theSumw2[bin] + ac2*his->theSumw2[bin];
    theBinEntries[bin] = ac1*theBinEntries[bin] + ac2*his->theBinEntries[bin];
    if( theBinSumw2.size() ) theBinSumw2[bin]  = ac1*ac1*ew1b + ac2*ac2*ew2[bin];
  }
  return true;
}

//----------------------------------------------------------------------
GmHistoProfile2 operator+(const GmHistoProfile2 &h1, const GmHistoProfile2 &h2)
{
  GmHistoProfile2 hnew = h1;
  hnew.Add(&h2,1);
  return hnew;
}

//----------------------------------------------------------------------
void GmHistoProfile2::Reset()
{
  GmHisto2::Reset();
  std::fill(theBinEntries.begin(), theBinEntries.end(), 0.);
  std::fill(theBinSumw2.begin(), theBinSumw2.end(), 0.);
  theTsumwz = theTsumwz2 = 0;
}

#endif
