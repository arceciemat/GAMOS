#ifndef GmMovementVerbosity_HH
#define GmMovementVerbosity_HH

#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

class GmMovementVerbosity : public GmVVerbosity
{
public:
  GmMovementVerbosity();

  virtual void SetFilterLevel( int fl );
  virtual int GetFilterLevel() const;

};

extern GmVerbosity MoveVerb;

#endif
