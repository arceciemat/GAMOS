#ifndef ShieldingVerbosity_HH
#define ShieldingVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class ShieldingVerbosity : public GmVVerbosity
{
public:
  ShieldingVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity SHVerb;

#endif
