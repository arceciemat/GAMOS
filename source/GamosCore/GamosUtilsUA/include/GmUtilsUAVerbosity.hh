#ifndef GmUtilsUAVerbosity_HH
#define GmUtilsUAVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmUtilsUAVerbosity : public GmVVerbosity
{
public:
  GmUtilsUAVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity UtilsUAVerb;

#endif
