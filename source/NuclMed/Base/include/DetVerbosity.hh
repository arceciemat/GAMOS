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
  #if defined GmDetBase_ALLOC_EXPORT 
    extern G4DLLEXPORT GmVerbosity DetVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity DetVerb;
  #endif
#else
 extern GmVerbosity DetVerb;
#endif


#endif
