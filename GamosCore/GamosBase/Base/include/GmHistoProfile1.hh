#ifndef GmHistoProfile1_hh
#define GmHistoProfile1_hh
#ifdef GAMOS_NO_ROOT

#include "GmHisto1.hh"

class GmHistoProfile1 : public GmHisto1
{
public:
  GmHistoProfile1( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  GmHistoProfile1( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge );
  ~GmHistoProfile1(){};

  void Fill( double valueX, double valueY, double weight = 1. );

  double GetBinContent(int ibin) const;
  double GetBinError(int ibin) const;

private:
  friend  GmHistoProfile1 operator+(const GmHistoProfile1 &h1, const GmHistoProfile1 &h2);
  
};

#endif

#endif
