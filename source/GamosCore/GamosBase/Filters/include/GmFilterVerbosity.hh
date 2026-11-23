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
  #if defined GamosCore_GamosBase_Filters_EXPORTS 
    extern __declspec(dllexport) GmVerbosity FilterVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity FilterVerb;
  #endif
#else
 extern GmVerbosity FilterVerb;
#endif

#endif
