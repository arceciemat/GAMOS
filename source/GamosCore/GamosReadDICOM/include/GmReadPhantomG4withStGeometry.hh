#ifndef GmReadPhantomG4withStGeometry_h
#define GmReadPhantomG4withStGeometry_h 1

#include "GmReadPhantomG4Geometry.hh"

#include <set>

class GmReadPhantomG4withStGeometry : public GmReadPhantomG4Geometry
{
public:

  GmReadPhantomG4withStGeometry();
  ~GmReadPhantomG4withStGeometry();

  virtual void ReadPV( GmFileIn& fin );

private:

};

#endif
