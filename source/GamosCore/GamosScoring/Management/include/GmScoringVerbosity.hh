#ifndef GmScoringVerbosity_HH
#define GmScoringVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmScoringVerbosity : public GmVVerbosity
{
public:
  GmScoringVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};


#ifdef WIN32
  #if defined GmScoringMgt_ALLOC_EXPORT 
    extern G4DLLEXPORT GmVerbosity ScoringVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity ScoringVerb;
  #endif
#else
  extern GmVerbosity ScoringVerb;
#endif

#endif
