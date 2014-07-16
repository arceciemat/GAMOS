#ifdef GAMOS_NO_ROOT

#include "GmHisto2.hh"

//----------------------------------------------------------------------
GmHisto2::GmHisto2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) 
  : theName(name), theNBinsX(nBinsX), theLowerEdgeX(lowerEdgeX), theUpperEdgeX(upperEdgeX), theNBinsY(nBinsY), theLowerEdgeY(lowerEdgeY), theUpperEdgeY(upperEdgeY)
{
  //-- Check consistency of data
  if(theNBinsX == 0) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The number of bins in Xaxis is 0 " << std::endl;
    std::exception();
  }

  if(theNBinsY == 0) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The number of bins in Yaxis is 0 " << std::endl;
    std::exception();
  }

  theBinWidthX = (theUpperEdgeX-theLowerEdgeX)/theNBinsX;
  theBinWidthY = (theUpperEdgeY-theLowerEdgeY)/theNBinsY;

  //-- Check consistency of data
  if(theBinWidthX == 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower and upper edges in X axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthX < 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower edge in X axis is bigger than the upper edge " << std::endl;
    std::exception();
  }
  if(theBinWidthY == 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower and upper edges in Y axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthY < 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower edge in Y axis is bigger than the upper edge " << std::endl;
    std::exception();
  }

  if( nBinsX > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins in axis X GmHisto2: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }
  if( nBinsY > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins in axis Y GmHisto2: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }

  theSumWX = 0.; 
  theSumWX2 = 0.; 
  theSumWY = 0.; 
  theSumWY2 = 0.; 
  theSumW = 0.; 
  theSumW2 = 0.; 
  theEntries = 0;
}

//----------------------------------------------------------------------
GmHisto2::GmHisto2( const std::string& name, const std::string& , int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) 
  : theName(name), theNBinsX(nBinsX), theLowerEdgeX(lowerEdgeX), theUpperEdgeX(upperEdgeX), theNBinsY(nBinsY), theLowerEdgeY(lowerEdgeY), theUpperEdgeY(upperEdgeY)
{
  //-- Check consistency of data
  if(theNBinsX == 0) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The number of bins in Xaxis is 0 " << std::endl;
    std::exception();
  }

  if(theNBinsY == 0) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The number of bins in Yaxis is 0 " << std::endl;
    std::exception();
  }

  theBinWidthX = (theUpperEdgeX-theLowerEdgeX)/theNBinsX;
  theBinWidthY = (theUpperEdgeY-theLowerEdgeY)/theNBinsY;

  //-- Check consistency of data
  if(theBinWidthX == 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower and upper edges in X axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthX < 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower edge in X axis is bigger than the upper edge " << std::endl;
    std::exception();
  }
  if(theBinWidthY == 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower and upper edges in Y axis are equal " << std::endl;
    std::exception();
  }
  if(theBinWidthY < 0.) {
    std::cerr << "!!! ERROR creating GmHisto2: " << name << "     The lower edge in Y axis is bigger than the upper edge " << std::endl;
    std::exception();
  }

  if( nBinsX > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins in axis X GmHisto2: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }
  if( nBinsY > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins in axis Y GmHisto2: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }

  theSumWX = 0.; 
  theSumWX2 = 0.; 
  theSumWY = 0.; 
  theSumWY2 = 0.; 
  theSumW = 0.; 
  theSumW2 = 0.; 
  theEntries = 0;
}


//----------------------------------------------------------------------
void GmHisto2::Fill( double valueX, double valueY, double weight )
{
  //--- Get bin
  int ibinX = int((valueX - theLowerEdgeX)/theBinWidthX) + 1;
  if( ibinX < 0 ) ibinX = 0;
  if( ibinX > int(theNBinsX+1) ) ibinX = theNBinsX+1;
  int ibinY = int((valueY - theLowerEdgeY)/theBinWidthY) + 1;
  if( ibinY < 0 ) ibinY = 0;
  if( ibinY > int(theNBinsY+1) ) ibinY = theNBinsY+1;

  int ibin = ibinX + (theNBinsX+2)*ibinY;
  theBinN[ibin] ++;
  theBinSumW[ibin] += weight;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  double w2 = weight*weight;
  theBinSumW2[ibin] += w2;

  if( valueX >= theLowerEdgeX && valueX <= theUpperEdgeX 
      && valueY >= theLowerEdgeY && valueY <= theUpperEdgeY ) {
    theSumWX += valueX*weight; 
    theSumWX2 += valueX*valueX*weight; 
    theSumWY += valueY*weight; 
    theSumWY2 += valueY*valueY*weight; 
    //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;
    //  theSumWY += valueY*weight; 
    // theSumWY2 += valueY*valueY*weight; 
    //  std::cout << " theSumWY2 " << theSumWY2 << " " << theSumWY2/theBinN[ibin] << std::endl;
    
    theSumW += weight;
    theSumW2 += w2;
  }

  theEntries++;
}


//----------------------------------------------------------------------
double GmHisto2::GetBinContent(int ibinX, int ibinY) const 
{
  if( ibinX < 0 ) {
    std::cerr << "GmHisto2::GetBinContent binX " << ibinX << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinY < 0 ) {
    std::cerr << "GmHisto2::GetBinContent binY " << ibinY << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinX > int(theNBinsX+2) ) {
    std::cerr << "GmHisto2::GetBinContent binX " << ibinX << "  is too big ( > " << theNBinsX+1 << std::endl;
    std::exception();
  }
  if( ibinY > int(theNBinsY+1) ) {
    std::cerr << "GmHisto2::GetBinContent binY " << ibinY << "  is too big ( > " << theNBinsY+1 << std::endl;
    std::exception();
  }
  
  int ibin = ibinX + (theNBinsX+2)*ibinY;
  std::map<int,double>::const_iterator ite = theBinSumW.find(ibin);
  if( ite != theBinSumW.end() ) {
    return (*ite).second;
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------------
double GmHisto2::GetBinError(int ibinX, int ibinY) const 
{
  if( ibinX < 0 ) {
    std::cerr << "GmHisto2::GetBinXError binX " << ibinX << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinY < 0 ) {
    std::cerr << "GmHisto2::GetBinError binY " << ibinY << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinX > int(theNBinsX+1) ) {
    std::cerr << "GmHisto2::GetBinError binX " << ibinX << "  is too big ( > " << theNBinsX+1 << std::endl;
    std::exception();
  }
  if( ibinY > int(theNBinsY+1) ) {
    std::cerr << "GmHisto2::GetBinError binY " << ibinY << "  is too big ( > " << theNBinsY+1 << std::endl;
    std::exception();
  }
  
  int ibin = ibinX + (theNBinsX+2)*ibinY;
  std::map<int,double>::const_iterator ite = theBinSumW2.find(ibin);
  if( ite != theBinSumW2.end() ) {
    return sqrt((*ite).second);
  } else {
    return 0.;
  }
}


//----------------------------------------------------------------------
int GmHisto2::GetEntries() const
{

  return theEntries;
  /*  int nbin = (theNBinsY+2) * (theNBinsX+2);

  int nent = 0;
  for( int ibin = 0; ibin < nbin; ibin++ ){
    std::map<int,int>::const_iterator ite = theBinN.find(ibin);
    if( ite != theBinN.end() ) nent += (*ite).second;
  }
  
  return nent;
  */
}

//----------------------------------------------------------------------
double GmHisto2::GmGenUtils::GetIntegral() const
{
  return theSumW;
}

//----------------------------------------------------------------------
int GmHisto2::GetBinX(float binx ) const
{
  int ibinX = int((binx-theLowerEdgeX)/theBinWidthX)+1;
  if( ibinX < 1 ){
    std::cerr << "!!! ERROR: GetBinX failed for histogram " << theName << " " << binx << " < " << theLowerEdgeX << std::endl;
  } else if( ibinX > int(theNBinsX) ){
    std::cerr << "!!! ERROR: GetBinX failed for histogram " << theName << " " << binx << " > " << theUpperEdgeX << std::endl;
  }

  return ibinX;
}

//----------------------------------------------------------------------
int GmHisto2::GetBinY(float biny ) const
{
  int ibinY = int((biny-theLowerEdgeY)/theBinWidthY)+1;
  if( ibinY < 1 ){
    std::cerr << "!!! ERROR: GetBinY failed for histogram " << theName << " " << biny << " < " << theLowerEdgeY << std::endl;
  } else if( ibinY > int(theNBinsY) ){
    std::cerr << "!!! ERROR: GetBinY failed for histogram " << theName << " " << biny << " > " << theUpperEdgeY << std::endl;
  }

  return ibinY;
}


//----------------------------------------------------------------------
double GmHisto2::GetBinLowEdgeX( int ibin ) const
{
  return theLowerEdgeX+theBinWidthX*ibin;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinUpEdgeX( int ibin ) const
{
  return theLowerEdgeX+theBinWidthX*(ibin+1);
}

//----------------------------------------------------------------------
double GmHisto2::GetBinLowEdgeY( int ibin ) const
{
  return theLowerEdgeY+theBinWidthY*ibin;
}

//----------------------------------------------------------------------
double GmHisto2::GetBinUpEdgeY( int ibin ) const
{
  return theLowerEdgeY+theBinWidthY*(ibin+1);
}
 

//----------------------------------------------------------------------
GmHisto2 operator+(const GmHisto2 &h1, const GmHisto2 &h2)
{
  GmHisto2* his = new GmHisto2( h1.GetName(), h1.GetNbinsX(), 
				h1.GetLowerEdgeX(), h1.GetUpperEdgeX(),
				h1.GetNbinsY(), 
				h1.GetLowerEdgeY(), h1.GetUpperEdgeY() );
  for(int ibinX = 0; ibinX <= h1.GetNbinsX()+1; ibinX++ ){
    for(int ibinY = 0; ibinY <= h1.GetNbinsY()+1; ibinY++ ){
      int ibin = ibinX + (h1.theNBinsX+2)*ibinY;
      his->theBinN[ibin] = h1.GetBinContent(ibinX,ibinY) + h2.GetBinContent(ibinX,ibinY);
      his->theBinSumW[ibin] = h2.GetBinSumW(ibin) + h2.GetBinSumW(ibin);
      his->theBinSumW2[ibin] = h1.GetBinSumW2(ibin) + h2.GetBinSumW2(ibin);
    }
  }

  his->theSumWX = h1.GetSumWX() + h2.GetSumWX();
  his->theSumWX2 = h1.GetSumWX2() + h2.GetSumWX2();
  his->theSumWY = h1.GetSumWY() + h2.GetSumWY();
  his->theSumWY2 = h1.GetSumWY2() + h2.GetSumWY2();
  his->theSumW = h1.GetSumW() + h2.GetSumW();
  his->theSumW2 = h1.GetSumW2() + h2.GetSumW2();
  
  return *his;
}

//----------------------------------------------------------------------
void GmHisto2::SetBinContent(int ibinX, int ibinY, double val)
{
  int ibin = ibinX + (theNBinsX+2)*ibinY;

  double oldVal =  theBinN[ibin];

  theBinN[ibin] = val;
  if( theBinSumW[ibin] == 0 ){ // not set before
    theSumW += 1;
    theSumW2 += 1;
  }
  theBinSumW[ibin] = 1;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  theBinSumW2[ibin] += 1;

  theSumWX += -oldVal + val; 
  theSumWX2 += -oldVal*oldVal + val*val; 
  //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;

}

//----------------------------------------------------------------------
void GmHisto2::SetBinError(int ibinX, int ibinY, double val)
{ 
  int ibin = ibinX + (theNBinsX+2)*ibinY;

  double oldVal =  theBinSumW[ibin];
  theBinSumW[ibin] = val;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  theBinSumW2[ibin] = val*val;

  theSumWX += -oldVal + val; 
  theSumWX2 += -oldVal*oldVal+val*val; 
  theSumWY += -oldVal + val; 
  theSumWY2 += -oldVal*oldVal+val*val; 
  //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;
  theSumW = -oldVal + val;
  theSumW2 +=  -oldVal*oldVal+val*val; 
}

//----------------------------------------------------------------------
double GmHisto2::GetEffectiveEntries() const
{
   // number of effective entries of the histogram,
   // i.e. the number of unweighted entries a histogram would need to 
   // have the same statistical power as this histogram with possibly 
   // weighted entries 

  std::cout << theName << "  GmHisto2::GetEffectiveEntries " << theSumW*theSumW/theSumW2 << " " << theSumW<< " " << theSumW2 << std::endl;
   return (theSumW2 ? theSumW*theSumW/theSumW2 : 0.);
}

//----------------------------------------------------------------------
double GmHisto2::GetMean( int axis ) const
{ 
  // return GetMean();
  if( axis == 1 ) {
    return ( theSumW ? theSumWX / theSumW : 0. );
  } else if( axis == 2 ) {
    return ( theSumW ? theSumWY / theSumW : 0. );
  }
}

//----------------------------------------------------------------------
double GmHisto2::GetMeanError( int axis ) const
{ 
 
  double rms = GetRMS(axis);
  double neff = GetEffectiveEntries();
  std::cout << theName << " GmHisto2::GetMeanError " << rms/sqrt(neff) << " " << rms << " " << neff << std::endl;
  return ( neff > 0 ? rms/sqrt(neff) : 0. ); 
}

//----------------------------------------------------------------------
double GmHisto2::GetRMS( int axis ) const
{ 
  if( axis == 1 ) { 
    double sumr = theSumWX/theSumW;
    return ( theSumW ? sqrt(theSumWX2/theSumW - (sumr*sumr) ) : 0. );
  } else if( axis == 2 ) {
    double sumr = theSumWY/theSumW;
    return ( theSumW ? sqrt(theSumWY2/theSumW - (sumr*sumr) ) : 0. );
  }

}

//----------------------------------------------------------------------
double GmHisto2::GetRMSError( int axis ) const
{
  double rms = GetRMS(axis);
  double neff = GetEffectiveEntries();
  return ( neff > 0 ? sqrt(rms*rms/(2*neff) ) : 0. ); 
}

#endif
