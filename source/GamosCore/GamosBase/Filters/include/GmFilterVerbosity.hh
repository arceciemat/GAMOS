#ifndef GmFilterVerbosity_hh
#define GmFilterVerbosity_hh

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmFilterVerbosity : public GmVVerbosity
{
public:
  GmFilterVerbosity();
  virtual ~GmFilterVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GmBaseBase_ALLOC_EXPORT 
    extern G4DLLEXPORT GmVerbosity FilterVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity FilterVerb;
  #endif
#else
 extern GmVerbosity FilterVerb;
#endif

#endif
