#ifdef GAMOS_NO_ROOT

#include "GmHisto1.hh"

//----------------------------------------------------------------------
GmHisto1::GmHisto1( const std::string& name, int nBins, double lowerEdge, double upperEdge ) 
  : theName(name), theNBins(nBins), theLowerEdge(lowerEdge), theUpperEdge(upperEdge)
{
  //-- Check consistency of data
  if(theNBins == 0) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The number of bins is 0 " << std::endl;
    std::exception();
  }

  theBinWidth = (theUpperEdge-theLowerEdge)/theNBins;

  //-- Check consistency of data
  if(theBinWidth == 0.) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The lower and upper edges are equal " << std::endl;
    std::exception();
  }
  if(theBinWidth < 0.) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The lower edge is bigger than the upper edge " << std::endl;
    std::exception();
  }

  if( nBins > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins GmHisto1: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }
  theSumWX = 0.; 
  theSumWX2 = 0.; 
  theSumW = 0.; 
  theSumW2 = 0.; 
  theEntries = 0;
}

//----------------------------------------------------------------------
GmHisto1::GmHisto1( const std::string& name, const std::string& , int nBins, double lowerEdge, double upperEdge ) 
  : theName(name), theNBins(nBins), theLowerEdge(lowerEdge), theUpperEdge(upperEdge)
{
  //-- Check consistency of data
  if(theNBins == 0) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The number of bins is 0 " << std::endl;
    std::exception();
  }

  theBinWidth = (theUpperEdge-theLowerEdge)/theNBins;

  //-- Check consistency of data
  if(theBinWidth == 0.) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The lower and upper edges are equal " << std::endl;
    std::exception();
  }
  if(theBinWidth < 0.) {
    std::cerr << "!!! ERROR creating GmHisto1: " << name << "     The lower edge is bigger than the upper edge " << std::endl;
    std::exception();
  }

  if( nBins > pow(2.,sizeof(int)*8 ) ){
    std::cerr << "!!! ERROR too many histogram bins GmHisto1: " << name << "     The maximum number is pow(2.,sizeof(int)*8 =  " << pow(2.,sizeof(int)*8) << std::endl;
    std::exception();
  }
  theSumWX = 0.; 
  theSumWX2 = 0.; 
  theSumW = 0.; 
  theSumW2 = 0.; 
  theEntries = 0;
}


//----------------------------------------------------------------------
void GmHisto1::Fill( double value, double weight )
{
  //--- Get bin
  int ibin = int((value - theLowerEdge)/theBinWidth) + 1;
  if( ibin < 0 ) ibin = 0;
  if( ibin > int(theNBins+1) ) ibin = theNBins+1;

  theBinN[ibin] ++;
  theBinSumW[ibin] += weight;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  double w2 = weight*weight;
  theBinSumW2[ibin] += w2;

  if( value >= theLowerEdge && value <= theUpperEdge ) {
    theSumWX += value*weight; 
    theSumWX2 += value*value*weight; 
    //  std::cout << theName << " theSumWX " << theSumWX << " theSumWX2 " << theSumWX2 << std::endl;
    theSumW += weight;
    theSumW2 += w2;
  }
  theEntries++;

}

//----------------------------------------------------------------------
double GmHisto1::GetBinContent(int ibin) const 
{
  //-  std::cerr << "GmHisto1::GetBinContent bin " << ibin << "  " <<  theName << std::endl;

  if( ibin < 0 ) {
    std::cerr << "GmHisto1::GetBinContent bin " << ibin << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibin > int(theNBins+1) ) {
    std::cerr << "GmHisto1::GetBinContent bin " << ibin << "  is too big ( > " << theNBins+1 << std::endl;
    std::exception();
  }
  
  std::map<int,double>::const_iterator ite = theBinSumW.find(ibin);
  if( ite != theBinSumW.end() ) {
    return (*ite).second;
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------------
double GmHisto1::GetBinError(int ibin) const 
{
  if( ibin < 0 ) {
    std::cerr << "GmHisto1::GetBinError bin " << ibin << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibin > int(theNBins+1) ) {
    std::cerr << "GmHisto1::GetBinError bin " << ibin << "  is too big ( > " << theNBins+1 << std::endl;
    std::exception();
  }
  
  std::map<int,double>::const_iterator ite = theBinSumW2.find(ibin);
  if( ite != theBinSumW2.end() ) {
    return sqrt((*ite).second);
  } else {
    return 0.;
  }
}


//----------------------------------------------------------------------
double GmHisto1::GetBinLowEdge( int ibin ) const
{
  return theLowerEdge+ theBinWidth*ibin;
}

//----------------------------------------------------------------------
double GmHisto1::GetBinUpEdge( int ibin ) const
{
  return theLowerEdge+ theBinWidth*(ibin+1);
}

//----------------------------------------------------------------------
int GmHisto1::GetEntries() const
{
  return theEntries;
  /*
  int nent = 0;
  for( int ibin = 0; ibin <= theNBins+1; ibin++ ){
    std::map<int,int>::const_iterator ite = theBinN.find(ibin);
    if( ite != theBinN.end() ) nent += (*ite).second;
  }

  return nent;*/
}

//----------------------------------------------------------------------
double GmHisto1::GetMean() const
{
  std::cout << theName << " GmHisto1::GetMean() " <<  theSumWX / theSumW << " " << theSumWX << " " << theSumW << std::endl;
  return ( theSumW ? theSumWX / theSumW : 0. );
}

//----------------------------------------------------------------------
double GmHisto1::GetMeanError() const
{
  double rms = GetRMS();
  double neff = GetEffectiveEntries();
  return ( neff > 0 ? rms/sqrt(neff) : 0. ); 
}


//----------------------------------------------------------------------
double GmHisto1::GetRMS() const
{
  double sumr = theSumWX/theSumW;
  return ( theSumW ? sqrt(theSumWX2/theSumW - (sumr*sumr) ) : 0. );
}
  
//----------------------------------------------------------------------
double GmHisto1::GetRMSError() const
{
  double rms = GetRMS();
  double neff = GetEffectiveEntries();
  return ( neff > 0 ? sqrt(rms*rms/(2*neff) ) : 0. );
}
  
//----------------------------------------------------------------------
double GmHisto1::GetEffectiveEntries() const
{
   // number of effective entries of the histogram,
   // i.e. the number of unweighted entries a histogram would need to 
   // have the same statistical power as this histogram with possibly 
   // weighted entries 

   return (theSumW2 ? theSumW*theSumW/theSumW2 : 0.);
}

//----------------------------------------------------------------------
double GmHisto1::GetIntegral() const
{
  return theSumW;
}

//----------------------------------------------------------------------
int GmHisto1::GetBin(double binx ) const
{
  int ibin = int((binx-theLowerEdge)/theBinWidth)+1;
  if( ibin < 1 ){
    std::cerr << "!!! ERROR: GetBin failed for histogram " << theName << " " << binx << " < " << theLowerEdge << std::endl;
  } else if( ibin > int(theNBins) ){
    std::cerr << "!!! ERROR: GetBin failed for histogram " << theName << " " << binx << " > " << theUpperEdge << std::endl;
  }

  return ibin;
}

//----------------------------------------------------------------------
GmHisto1 operator+(const GmHisto1 &h1, const GmHisto1 &h2)
{
  GmHisto1* his = new GmHisto1( h1.GetName(), h1.GetNbins(), h1.GetLowerEdge(), h1.GetUpperEdge() );
  for(int ibin = 0; ibin <= h1.GetNbins()+1; ibin++ ){
    his->theBinN[ibin] = h1.GetBinContent(ibin) + h2.GetBinContent(ibin);
    his->theBinSumW[ibin] = h1.GetBinSumW(ibin) + h2.GetBinSumW(ibin);
    his->theBinSumW2[ibin] = h1.GetBinSumW2(ibin) + h2.GetBinSumW2(ibin);
  }
  his->theSumWX = h1.GetSumWX() + h2.GetSumWX();
  his->theSumWX2 = h1.GetSumWX2() + h2.GetSumWX2();
  his->theSumW = h1.GetSumW() + h2.GetSumW();
  his->theSumW2 = h1.GetSumW2() + h2.GetSumW2();
  
  return *his;
}

//----------------------------------------------------------------------
void GmHisto1::SetBinContent(int ibin, double val)
{
  double oldVal =  theBinN[ibin];

  theBinN[ibin] = val;
  if( theBinSumW[ibin] == 0 ){ // not set before
    theSumW += 1;
    theSumW2 += 1;
  }
  theBinSumW[ibin] = val;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  theBinSumW2[ibin] = val*val;

  theSumWX += -oldVal + val; 
  theSumWX2 += -oldVal*oldVal + val*val; 
  //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;

}

//----------------------------------------------------------------------
void GmHisto1::AddBinContent(int ibin, double val)
{
  double newVal = theBinN[ibin] + val;
  theBinN[ibin] = newVal;
  if( theBinSumW[ibin] == 0 ){ // not set before
    theSumW += 1;
    theSumW2 += 1;
  }
  theBinSumW[ibin] = newVal;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  theBinSumW2[ibin] += newVal*newVal;

  theSumWX += val; 
  theSumWX2 += val*val; 
  //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;

}

//----------------------------------------------------------------------
void GmHisto1::SetBinError(int ibin, double val)
{
  double oldVal =  theBinSumW[ibin];
  theBinSumW[ibin] = val;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  theBinSumW2[ibin] = val*val;

  theSumWX += -oldVal + val; 
  theSumWX2 += -oldVal*oldVal+val*val; 
  //  std::cout << " theSumWX2 " << theSumWX2 << " " << theSumWX2/theBinN[ibin] << std::endl;
  theSumW = -oldVal + val;
  theSumW2 +=  -oldVal*oldVal+val*val; 
}

//----------------------------------------------------------------------
double GmHisto1::GetBinCenter( int ibin ) const
{

  return theLowerEdge + theBinWidth*(ibin-0.5);
}


//----------------------------------------------------------------------                                         
void GmHisto1::Reset()
{
	theEntries = 0;
	theBinN.clear();
	theBinSumW.clear();
	theBinSumW2.clear();

	theSumW = 0;
	theSumW2 = 0;
	theSumWX = 0;
	theSumWX2 = 0;
}
#endif
