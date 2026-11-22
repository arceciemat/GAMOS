#ifndef GmDNAVerbosity_HH
#define GmDNAVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmDNAVerbosity : public GmVVerbosity
{
public:
  GmDNAVerbosity();
  virtual ~GmDNAVerbosity(){};

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity DNAVerb;

#endif
