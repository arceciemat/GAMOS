#ifndef GmHistoProfile2_hh
#define GmHistoProfile2_hh
#ifdef GAMOS_NO_ROOT

#include "GmHisto2.hh"

class GmHistoProfile2 : public GmHisto2
{
public:
  GmHistoProfile2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHistoProfile2( const std::string& name,const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  ~GmHistoProfile2(){};

  void Fill( double valueX, double valueY, double valueZ, double weight = 1. );

  double GetBinContent(int ibinX, int ibinY) const;
  double GetBinError(int ibinX, int ibinY) const;

private:
  friend GmHistoProfile2 operator+(const GmHistoProfile2 &h1, const GmHistoProfile2 &h2);
  
};

#endif

#endif
