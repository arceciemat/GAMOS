#ifndef GmGenerVerbosity_HH
#define GmGenerVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmGenerVerbosity : public GmVVerbosity
{
public:
  GmGenerVerbosity();
  virtual ~GmGenerVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity GenerVerb;

#endif
