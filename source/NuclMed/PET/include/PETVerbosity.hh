#ifndef PETVerbosity_HH
#define PETVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class PETVerbosity : public GmVVerbosity
{
public:
  PETVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity PETVerb;

#endif
