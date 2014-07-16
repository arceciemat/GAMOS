#ifdef GAMOS_NO_ROOT

#include "GmHistoProfile1.hh"

//----------------------------------------------------------------------
GmHistoProfile1::GmHistoProfile1( const std::string& name, int nBins, double lowerEdge, double upperEdge ) 
  : GmHisto1( name, nBins, lowerEdge, upperEdge)
{
}

//----------------------------------------------------------------------
GmHistoProfile1::GmHistoProfile1( const std::string& name, const std::string& , int nBins, double lowerEdge, double upperEdge ) 
  : GmHisto1( name, nBins, lowerEdge, upperEdge)
{
}


//----------------------------------------------------------------------
void GmHistoProfile1::Fill( double valueX, double valueY, double weight )
{
  std::cout << "  GmHistoProfile1::Fill " <<  valueX << " " <<  valueY << " " <<  weight <<std::endl;

  double weightY = valueY*weight;

  //--- Get bin
  int ibin = int((valueX - theLowerEdge)/theBinWidth) + 1;
  if( ibin < 0 ) ibin = 0;
  if( ibin > int(theNBins+1) ) ibin = theNBins+1;

  theBinN[ibin] ++;
  theBinSumW[ibin] += weightY;
  //   std::cout << " FILL GmHISTO1 " << ibin << " = " << weight << std::endl;
  double wY2 = weightY*weightY;
  theBinSumW2[ibin] += wY2;

  if( valueX >= theLowerEdge && valueX <= theUpperEdge ) {
    theSumWX += valueX*weight; 
    theSumWX2 += valueX*valueX*weight; 
    std::cout << theName << " theSumWX " << theSumWX << " theSumWX2 " << theSumWX2 << std::endl;
    theSumW += weight;
    theSumW2 += weight*weight;
  }
}


//----------------------------------------------------------------------
double GmHistoProfile1::GetBinContent(int ibin) const 
{
  if( ibin < 0 ) {
    std::cerr << "GmHistoProfile1::GetBinContent bin " << ibin << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibin > int(theNBins+1) ) {
    std::cerr << "GmHistoProfile1::GetBinContent bin " << ibin << "  is too big ( > " << theNBins+1 << std::endl;
    std::exception();
  }
  
  std::map<int,double>::const_iterator iteV = theBinSumW.find(ibin);
  std::map<int,int>::const_iterator iteN = theBinN.find(ibin);
  if( iteV != theBinSumW.end() ) {
    std::cout << " GmHistoProfile1::GetBinContent " << ibin << " " << (*iteV).second/(*iteN).second << " " <<  (*iteV).second << " "<< (*iteN).second << std::endl; 
    return (*iteV).second/(*iteN).second;
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------------
double GmHistoProfile1::GetBinError(int ibin) const 
{
  if( ibin < 0 ) {
    std::cerr << "GmHistoProfile1::GetBinError bin " << ibin << "  is less than 0 " << std::endl;
    std::exception();
  }
  if( ibin > int(theNBins+1) ) {
    std::cerr << "GmHistoProfile1::GetBinError bin " << ibin << "  is too big ( > " << theNBins+1 << std::endl;
    std::exception();
  }
  
  std::map<int,int>::const_iterator iteN = theBinN.find(ibin);
  std::map<int,double>::const_iterator iteV = theBinSumW.find(ibin);
  std::map<int,double>::const_iterator iteV2 = theBinSumW2.find(ibin);

   if( iteV2 != theBinSumW2.end() ) {
     std::cout <<  " GmHistoProfile1::GetBinError " << sqrt( ((*iteV2).second*(*iteN).second - (*iteV).second*(*iteV).second )/((*iteN).second) ) / (*iteN).second << " " << (*iteV2).second << " " << (*iteV).second << " " << (*iteN).second << std::endl;
    return sqrt( ((*iteV2).second*(*iteN).second - (*iteV).second*(*iteV).second )/((*iteN).second) ) / (*iteN).second;
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------------
GmHistoProfile1 operator+(const GmHistoProfile1 &h1, const GmHistoProfile1 &h2)
{
  GmHistoProfile1* his = new GmHistoProfile1( h1.GetName(), h1.GetNbins(), h1.GetLowerEdge(), h1.GetUpperEdge() );
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

#endif
