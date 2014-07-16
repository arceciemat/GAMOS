#ifdef GAMOS_NO_ROOT

#include "GmHistoProfile2.hh"

//----------------------------------------------------------------------
GmHistoProfile2::GmHistoProfile2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) 
  : GmHisto2(name, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY)
{
}

//----------------------------------------------------------------------
GmHistoProfile2::GmHistoProfile2( const std::string& name, const std::string& , int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY ) 
  : GmHisto2(name, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY)
{
}


//----------------------------------------------------------------------
void GmHistoProfile2::Fill( double valueX, double valueY, double valueZ, double weight )
{

  double weightZ = valueZ*weight;

  //--- Get bin
  int ibinX = int((valueX - theLowerEdgeX)/theBinWidthX) + 1;
  if( ibinX < 0 ) ibinX = 0;
  if( ibinX > int(theNBinsX+1) ) ibinX = theNBinsX+1;
  int ibinY = int((valueY - theLowerEdgeY)/theBinWidthY) + 1;
  if( ibinY < 0 ) ibinY = 0;
  if( ibinY > int(theNBinsY+1) ) ibinY = theNBinsY+1;

  int ibin = ibinX + (theNBinsX+2)*ibinY;
  theBinN[ibin] ++;
  theBinSumW[ibin] += weightZ;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  double wZ2 = weightZ*weightZ;
  theBinSumW2[ibin] += wZ2;

  if( valueX >= theLowerEdgeX && valueX <= theUpperEdgeX 
      && valueY >= theLowerEdgeY && valueY <= theUpperEdgeY ) {
    theSumWX += valueX*weight; 
    theSumWX2 += valueX*valueX*weight; 
    theSumWY += valueY*weight; 
    theSumWY2 += valueY*valueY*weight; 
    
    theSumW += weight;
    theSumW2 += weight*weight;
  }
}


//----------------------------------------------------------------------
double GmHistoProfile2::GetBinContent(int ibinX, int ibinY) const 
{
  if( ibinX < 0 ) {
    std::cerr << "GmHistoProfile2::GetBinContent binX " << ibinX << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinY < 0 ) {
    std::cerr << "GmHistoProfile2::GetBinContent binY " << ibinY << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinX > int(theNBinsX+1) ) {
    std::cerr << "GmHistoProfile2::GetBinContent binX " << ibinX << "  is too big ( > " << theNBinsX+1 << std::endl;
    std::exception();
  }
  if( ibinY > int(theNBinsY+1) ) {
    std::cerr << "GmHistoProfile2::GetBinContent binY " << ibinY << "  is too big ( > " << theNBinsY+1 << std::endl;
    std::exception();
  }
  
  int ibin = ibinX + (theNBinsX+2)*ibinY;
  std::map<int,double>::const_iterator iteV = theBinSumW.find(ibin);
  std::map<int,int>::const_iterator iteN = theBinN.find(ibin);
  if( iteV != theBinSumW.end() ) {
    return (*iteV).second/(*iteN).second;
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------------
double GmHistoProfile2::GetBinError(int ibinX, int ibinY) const 
{
  if( ibinX < 0 ) {
    std::cerr << "GmHistoProfile2::GetBinXError binX " << ibinX << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinY < 0 ) {
    std::cerr << "GmHistoProfile2::GetBinError binY " << ibinY << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibinX > int(theNBinsX+1) ) {
    std::cerr << "GmHistoProfile2::GetBinError binX " << ibinX << "  is too big ( > " << theNBinsX+1 << std::endl;
    std::exception();
  }
  if( ibinY > int(theNBinsY+1) ) {
    std::cerr << "GmHistoProfile2::GetBinError binY " << ibinY << "  is too big ( > " << theNBinsY+1 << std::endl;
    std::exception();
  }
  
  int ibin = ibinX + (theNBinsX+2)*ibinY;
  std::map<int,int>::const_iterator iteN = theBinN.find(ibin);
  std::map<int,double>::const_iterator iteV = theBinSumW.find(ibin);
  std::map<int,double>::const_iterator iteV2 = theBinSumW2.find(ibin);
  if( iteV2 != theBinSumW2.end() ) {
    return sqrt( ((*iteV2).second*(*iteN).second - (*iteV).second*(*iteV).second )/((*iteN).second) ) / (*iteN).second;
  } else {
    return 0.;
  }
}


//----------------------------------------------------------------------
GmHistoProfile2 operator+(const GmHistoProfile2 &h1, const GmHistoProfile2 &h2)
{
  GmHistoProfile2* his = new GmHistoProfile2( h1.GetName(), h1.GetNbinsX(), 
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

#endif
