#ifndef GmDataVerbosity_HH
#define GmDataVerbosity_HH

#include "G4Types.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmDataVerbosity : public GmVVerbosity
{
public:
  GmDataVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GmDataMgt_ALLOC_EXPORT 
    extern G4DLLEXPORT GmVerbosity DataVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity DataVerb;
  #endif
#else
 extern GmVerbosity DataVerb;
#endif

#endif
