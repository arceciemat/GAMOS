#ifndef SPECTVerbosity_HH
#define SPECTVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class SPECTVerbosity : public GmVVerbosity
{
public:
  SPECTVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity SPECTVerb;

#endif
