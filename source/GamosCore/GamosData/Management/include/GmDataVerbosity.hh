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
  #if defined GamosCore_GamosData_Management_EXPORTS 
    extern __declspec(dllexport) GmVerbosity DataVerb;
  #else 
    extern __declspec(dllimport) GmVerbosity DataVerb;
  #endif
#else
 extern GmVerbosity DataVerb;
#endif

#endif
