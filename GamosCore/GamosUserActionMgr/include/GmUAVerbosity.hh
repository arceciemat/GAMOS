#ifndef GmUAVerbosity_HH
#define GmUAVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmUAVerbosity : public GmVVerbosity
{
public:
  GmUAVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity UAVerb;

#endif
