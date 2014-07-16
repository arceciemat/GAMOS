#ifndef GmReadPhantomEGSwithPSGeometry_h
#define GmReadPhantomEGSwithPSGeometry_h 1

#include "GmReadPhantomEGSGeometry.hh"

#include <set>

class GmReadPhantomEGSwithPSGeometry : public GmReadPhantomEGSGeometry
{
public:

  GmReadPhantomEGSwithPSGeometry();
  ~GmReadPhantomEGSwithPSGeometry();

  virtual void ReadPV( std::ifstream& fin );

private:

};

#endif
