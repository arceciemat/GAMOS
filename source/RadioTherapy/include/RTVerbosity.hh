#ifndef RTVerbosity_HH
#define RTVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class RTVerbosity : public GmVVerbosity
{
public:
  RTVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};
#ifdef WIN32
  #if defined RadioTherapy_EXPORTS 
    extern __declspec(dllexport) GmVerbosity RTVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity RTVerb;
  #endif
#else
 extern GmVerbosity RTVerb;
#endif

#endif
