#ifndef GmBiasingVerbosity_HH
#define GmBiasingVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmBiasingVerbosity : public GmVVerbosity
{
public:
  GmBiasingVerbosity();
  virtual ~GmBiasingVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GamosCore_GamosPhysics_Biasing_EXPORTS 
    extern __declspec(dllexport) GmVerbosity BiasingVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity BiasingVerb;
  #endif
#else
extern GmVerbosity BiasingVerb;
#endif

#endif
