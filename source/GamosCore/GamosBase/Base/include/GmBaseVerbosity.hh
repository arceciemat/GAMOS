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
  #if defined GamosCore_GamosBase_Base_EXPORTS 
    extern __declspec(dllexport) GmVerbosity BaseVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity BaseVerb;
  #endif
#else
 extern GmVerbosity BaseVerb;
#endif

#endif
