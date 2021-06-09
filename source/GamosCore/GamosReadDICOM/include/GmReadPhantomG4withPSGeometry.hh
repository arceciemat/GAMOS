#ifndef GmReadPhantomG4withPSGeometry_h
#define GmReadPhantomG4withPSGeometry_h 1

#include "GmReadPhantomG4Geometry.hh"

#include <set>

class GmReadPhantomG4withPSGeometry : public GmReadPhantomG4Geometry
{
public:

  GmReadPhantomG4withPSGeometry();
  ~GmReadPhantomG4withPSGeometry();

  virtual void ReadPV( std::ifstream& fin );

private:

};

#endif
