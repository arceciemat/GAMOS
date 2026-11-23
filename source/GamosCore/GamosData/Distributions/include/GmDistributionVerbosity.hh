#ifndef GmDistributionVerbosity_HH
#define GmDistributionVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmDistributionVerbosity : public GmVVerbosity
{
public:
  GmDistributionVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity DistVerb;

#endif
