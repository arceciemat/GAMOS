#ifndef GmPhysicsVerbosity_HH
#define GmPhysicsVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmPhysicsVerbosity : public GmVVerbosity
{
public:
  GmPhysicsVerbosity();
  virtual ~GmPhysicsVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

#ifdef WIN32
  #if defined GmPhysicsPL_ALLOC_EXPORT
    extern G4DLLEXPORT GmVerbosity PhysicsVerb;
  #else 
    extern G4DLLIMPORT GmVerbosity PhysicsVerb;
  #endif
#else
extern GmVerbosity PhysicsVerb;
#endif


#endif
