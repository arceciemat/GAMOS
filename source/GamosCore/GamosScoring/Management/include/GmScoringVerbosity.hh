#ifndef GmScoringVerbosity_HH
#define GmScoringVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmScoringVerbosity : public GmVVerbosity
{
public:
  GmScoringVerbosity();
  virtual ~GmScoringVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GamosCore_GamosScoring_Management_EXPORTS 
    extern __declspec(dllexport) GmVerbosity ScoringVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity ScoringVerb;
  #endif
#else
  extern GmVerbosity ScoringVerb;
#endif

#endif
