#ifndef GmSDVerbosity_HH
#define GmSDVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmSDVerbosity : public GmVVerbosity
{
public:
  GmSDVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity SDVerb;

#endif
