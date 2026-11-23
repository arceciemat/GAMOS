#ifndef CCVerbosity_HH
#define CCVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class CCVerbosity : public GmVVerbosity
{
public:
  CCVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity CCVerb;

#endif
