#ifndef GmAnalysisVerbosity_HH
#define GmAnalysisVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmAnalysisVerbosity : public GmVVerbosity
{
public:
  GmAnalysisVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity AnaVerb;

#endif
