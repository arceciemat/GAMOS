#ifndef DetVerbosity_HH
#define DetVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class DetVerbosity : public GmVVerbosity
{
public:
  DetVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined NuclMed_Base_EXPORTS 
    extern __declspec(dllexport) GmVerbosity DetVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity DetVerb;
  #endif
#else
 extern GmVerbosity DetVerb;
#endif


#endif
