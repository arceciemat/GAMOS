#ifndef GmClassifierVerbosity_hh
#define GmClassifierVerbosity_hh

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmClassifierVerbosity : public GmVVerbosity
{
public:
  GmClassifierVerbosity();
  virtual ~GmClassifierVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GamosCore_GamosBase_Classifiers_EXPORTS 
    extern __declspec(dllexport) GmVerbosity ClassifierVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity ClassifierVerb;
  #endif
#else
 extern GmVerbosity ClassifierVerb;
#endif

#endif
