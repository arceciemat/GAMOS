#ifndef readExemption_C
#define readExemption_C

#include <fstream>
#include <string>
#include <map>
std::map<std::string,double> theExemption;
#include "../GetWordsInLine.C"
#ifndef GAMOS_NO_ROOT
#include "../PlotData/PlotUtils.C"
#else
#include "ROOTUtilities/PlotData/PlotDataSVG.cc"
#endif

//-------------------------------------------------------------------------
void readExemption(std::string fileName)
{
  std::ifstream fin(fileName.c_str());
  
  std::vector<std::string> wl;
  for(int ii = 0;;ii++) {
    if( !GetWordsInLine(fin,wl) ) break;
    
    if( wl.size() == 3 ) {
      theExemption[wl[0]] = atof(wl[1].c_str());
    }
    //    std::cout << " READ EXEMPTION " << wl[0] << " = " << atof(wl[1].c_str()) << std::endl;
  }  
  
}    

#ifndef GAMOS_NO_ROOT
//-------------------------------------------------------------------------
void DrawExemptionLine( TH1F* his, std::string isotName, EColor color, double& xInitExemp )
{
  //--- clean isotope name
  size_t ic = isotName.find("[");
  if( ic != std::string::npos ) {
    isotName = isotName.substr(0,ic);
  }
  CalculateHistoDims(his);
  // Draw exemption line
  std::map<std::string,double>::const_iterator iteExem = theExemption.find(isotName);
  //      std::cout << " EXEMPTION " << theExemption.size() << " " << isotName << std::endl;
  double exemption = DBL_MAX;
  if( iteExem != theExemption.end() ){
    exemption = (*iteExem).second;
    TLine *exemLine = new TLine(hxMin, exemption, hxMax, exemption );
    exemLine->SetLineColor(color);
    exemLine->SetLineStyle(2);
    exemLine->SetLineWidth(1);
    exemLine->Draw();

    double xPos = GetXPos(xInitExemp, hxMin, hxWidth );
    TText* text = new TText(xPos, exemption, isotName.c_str());
    text->SetTextColor(color);
    text->SetTextSize(0.02);
    text->Draw();
    UInt_t tx = 1;
    UInt_t ty = 1;
    text->GetTextExtent(tx,ty,isotName.c_str());
    xInitExemp += tx + 5;
  } else {
    if( verbose >= 0 ) std::cerr << " DrawExemptionLine: isotope not found in exemption file: " << isotName << std::endl;
  }
   
}

#else
//-------------------------------------------------------------------------
//--- SVG version: dashed horizontal line at the exemption value, with the isotope name
void DrawExemptionLine( CDHisto* his, std::string isotName, EColor color, double& xInitExemp )
{
  //--- clean isotope name
  size_t ic = isotName.find("[");
  if( ic != std::string::npos ) {
    isotName = isotName.substr(0,ic);
  }
  std::map<std::string,double>::const_iterator iteExem = theExemption.find(isotName);
  if( iteExem != theExemption.end() ){
    double exemption = (*iteExem).second;
    std::string col = ColorToSVG(color);
    his->DrawLine( his->GetXmin(), exemption, his->GetXmax(), exemption, col, 1., GmSVGCanvas::RootLineStyleToDash(2) );
    if( his->IsInsideY(exemption) ) {
      double size = 0.02;
      double ty = GetTextYExtent( his, size );
      theCanvas->Text( PointsToPixX(his,xInitExemp), his->ToPixY(exemption), isotName, ty, col, "start" );
      xInitExemp += GetTextXExtent( his, isotName, size ) + 5;
    }
  } else {
    if( verbose >= 0 ) std::cerr << " DrawExemptionLine: isotope not found in exemption file: " << isotName << std::endl;
  }
}
#endif

 #endif
