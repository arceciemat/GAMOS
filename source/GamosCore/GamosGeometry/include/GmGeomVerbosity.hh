#ifndef GmGeomVerbosity_HH
#define GmGeomVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmGeomVerbosity : public GmVVerbosity
{
public:
  GmGeomVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity GeomVerb;

#endif
