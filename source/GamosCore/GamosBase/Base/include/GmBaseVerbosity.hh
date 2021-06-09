#ifndef GmBaseVerbosity_hh
#define GmBaseVerbosity_hh

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmBaseVerbosity : public GmVVerbosity
{
public:
  GmBaseVerbosity();
  virtual ~GmBaseVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GmBaseBase_ALLOC_EXPORT 
    extern G4DLLEXPORT GmVerbosity BaseVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity BaseVerb;
  #endif
#else
 extern GmVerbosity BaseVerb;
#endif

#endif
