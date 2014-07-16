#ifndef GmReadDICOMVerbosity_HH
#define GmReadDICOMVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmReadDICOMVerbosity : public GmVVerbosity
{
public:
  GmReadDICOMVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity ReadDICOMVerb;

#endif
