#ifndef GmHistoWriter_h
#define GmHistoWriter_h 1

#ifndef GAMOS_NO_ROOT
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#else
class GmHisto1;
class GmHisto2;
class GmHistoProfile1;
class GmHistoProfile2;
#endif

#include "globals.hh"

class GmHistoWriter 
{
public:
  GmHistoWriter(){};
  virtual ~GmHistoWriter(){};
#ifndef GAMOS_NO_ROOT
  virtual void SaveHisto1( const TH1F* his ) = 0;
  virtual void SaveHisto2( const TH2F* his ) = 0;
  virtual void SaveHistoProf1( const TProfile* his ) = 0;
  virtual void SaveHistoProf2( const TProfile2D* his ) = 0;
#else
  virtual void SaveHisto1( const GmHisto1* his ) = 0;
  virtual void SaveHisto2( const GmHisto2* his ) = 0;
  virtual void SaveHistoProf1( const GmHistoProfile1* his ) = 0;
  virtual void SaveHistoProf2( const GmHistoProfile2* his ) = 0;
#endif

 private:

};


#endif
