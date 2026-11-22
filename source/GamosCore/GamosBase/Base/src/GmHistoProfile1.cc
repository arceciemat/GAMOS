#ifndef GAMOS_ROOT

#include "GmHistoProfile1.hh"
#include <algorithm>

// The algorithms reproduce those of ROOT 6.26 classes TProfile and TProfileHelper (default error option: error on the mean,
// no y limits, no buffer, statistics without underflow/overflow)

//----------------------------------------------------------------------
GmHistoProfile1::GmHistoProfile1( const std::string& name, int nBins, double lowerEdge, double upperEdge )
  : GmHisto1( name, nBins, lowerEdge, upperEdge)
{
  theBinEntries.assign(theNcells,0.);
  theSumw2.assign(theNcells,0.);
  theBinSumw2.clear();
  theTsumwy = theTsumwy2 = 0;
}

//----------------------------------------------------------------------
GmHistoProfile1::GmHistoProfile1( const std::string& name, const std::string& , int nBins, double lowerEdge, double upperEdge )
  : GmHisto1( name, nBins, lowerEdge, upperEdge)
{
  theBinEntries.assign(theNcells,0.);
  theSumw2.assign(theNcells,0.);
  theBinSumw2.clear();
  theTsumwy = theTsumwy2 = 0;
}

//----------------------------------------------------------------------
int GmHistoProfile1::Fill( double valueX, double valueY )
{
  theEntries++;
  int bin = FindBin(valueX);
  AddBinContent(bin, valueY);
  theSumw2[bin] += valueY*valueY;
  theBinEntries[bin] += 1;
  if( theBinSumw2.size() ) theBinSumw2[bin] += 1;
  if( bin == 0 || bin > theNBins ) return -1;
  theTsumw++;
  theTsumw2++;
  theTsumwx  += valueX;
  theTsumwx2 += valueX*valueX;
  theTsumwy  += valueY;
  theTsumwy2 += valueY*valueY;
  return bin;
}

//----------------------------------------------------------------------
int GmHistoProfile1::Fill( double valueX, double valueY, double weight )
{
  double u = weight;
  theEntries++;
  int bin = FindBin(valueX);
  AddBinContent(bin, u*valueY);
  theSumw2[bin] += u*valueY*valueY;
  if( !theBinSumw2.size() && u != 1.0 ) Sumw2();  // must be called before accumulating the entries
  if( theBinSumw2.size() ) theBinSumw2[bin] += u*u;
  theBinEntries[bin] += u;
  if( bin == 0 || bin > theNBins ) return -1;
  theTsumw   += u;
  theTsumw2  += u*u;
  theTsumwx  += u*valueX;
  theTsumwx2 += u*valueX*valueX;
  theTsumwy  += u*valueY;
  theTsumwy2 += u*valueY*valueY;
  return bin;
}

//----------------------------------------------------------------------
void GmHistoProfile1::AddBinContent(int ibin)
{
  ++theArray[ibin];
}

//----------------------------------------------------------------------
void GmHistoProfile1::AddBinContent(int ibin, double val)
{
  theArray[ibin] += val;
}

//----------------------------------------------------------------------
double GmHistoProfile1::GetBinContent(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  if( theBinEntries[ibin] == 0 ) return 0;
  return theArray[ibin]/theBinEntries[ibin];
}

//----------------------------------------------------------------------
double GmHistoProfile1::GetBinEntries(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  return theBinEntries[ibin];
}

//----------------------------------------------------------------------
double GmHistoProfile1::GetBinEffectiveEntries(int ibin) const
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
double GmHistoProfile1::GetBinError(int ibin) const
{
  if( ibin < 0 || ibin >= theNcells ) return 0;
  double cont = theArray[ibin];        // sum of bin w *y
  double sum  = theBinEntries[ibin];   // sum of bin weights
  double err2 = theSumw2[ibin];        // sum of bin w * y^2
  double neff = GetBinEffectiveEntries(ibin);  // (sum of w)^2 / (sum of w^2)
  if( sum == 0 ) return 0;      // for empty bins
  double contsum = cont/sum;
  double eprim2  = fabs(err2/sum - contsum*contsum);
  double eprim   = sqrt(eprim2);
  // return standard error on the mean of y
  return eprim/sqrt(neff);
}

//----------------------------------------------------------------------
void GmHistoProfile1::Sumw2( bool flag )
{
  if( !flag ) {
    theBinSumw2.clear();
    return;
  }

  if( int(theBinSumw2.size()) == theNcells ) {
    G4cerr << "Warning in <GmHistoProfile1::Sumw2>: Sum of squares of profile bin weights structure already created " << theName << std::endl;
    return;
  }

  theBinSumw2 = theBinEntries;
}

//----------------------------------------------------------------------
void GmHistoProfile1::GetStats(double* stats) const
{
  if( theTsumw == 0 ) {
    // recalculate statistics from bin contents
    for( int ii = 0; ii < 6; ii++ ) stats[ii] = 0;
    for( int binx = 1; binx <= theNBins; binx++ ) {
      double w   = theBinEntries[binx];
      double w2  = (theBinSumw2.size() ? theBinSumw2[binx] : w);
      double x   = GetBinCenter(binx);
      stats[0] += w;
      stats[1] += w2;
      stats[2] += w*x;
      stats[3] += w*x*x;
      stats[4] += theArray[binx];
      stats[5] += theSumw2[binx];
    }
  } else {
    if( theTsumwy == 0 && theTsumwy2 == 0 ) {
      //this case may happen when processing TProfiles with version <=3
      for( int binx = 1; binx <= theNBins; binx++ ) {
	theTsumwy  += theArray[binx];
	theTsumwy2 += theSumw2[binx];
      }
    }
    stats[0] = theTsumw;
    stats[1] = theTsumw2;
    stats[2] = theTsumwx;
    stats[3] = theTsumwx2;
    stats[4] = theTsumwy;
    stats[5] = theTsumwy2;
  }
}

//----------------------------------------------------------------------
void GmHistoProfile1::PutStats(double* stats)
{
  theTsumw   = stats[0];
  theTsumw2  = stats[1];
  theTsumwx  = stats[2];
  theTsumwx2 = stats[3];
  theTsumwy  = stats[4];
  theTsumwy2 = stats[5];
}

//----------------------------------------------------------------------
void GmHistoProfile1::Scale(double fac)
{
  // as TProfileHelper::Scale: statistics are not changed
  double afac = fabs(fac);
  for( int bin = 0; bin < theNcells; bin++ ) {
    theArray[bin] = fac*theArray[bin];
    theSumw2[bin] = afac*afac*theSumw2[bin];
  }
}

//----------------------------------------------------------------------
bool GmHistoProfile1::Add(const GmHisto1* his1, double fac)
{
  // as TProfile::Add(h1,c1), that calls TProfileHelper::Add(this, this, h1, 1, c1)
  const GmHistoProfile1* his = dynamic_cast<const GmHistoProfile1*>(his1);
  if( !his ) {
    G4cerr << "Error in <GmHistoProfile1::Add>: Attempt to add a non-existing profile or a non-profile object" << std::endl;
    return false;
  }
  if( his->GetNbins() != theNBins ) {
    G4cerr << "Error in <GmHistoProfile1::Add>: Attempt to add profiles with different number of bins" << std::endl;
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
GmHistoProfile1 operator+(const GmHistoProfile1 &h1, const GmHistoProfile1 &h2)
{
  GmHistoProfile1 hnew = h1;
  hnew.Add(&h2,1);
  return hnew;
}

//----------------------------------------------------------------------
void GmHistoProfile1::Reset()
{
  GmHisto1::Reset();
  std::fill(theBinEntries.begin(), theBinEntries.end(), 0.);
  std::fill(theBinSumw2.begin(), theBinSumw2.end(), 0.);
  theTsumwy  = 0;
  theTsumwy2 = 0;
}

#endif
