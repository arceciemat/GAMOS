#ifndef GmReadPhantomEGSwithStGeometry_h
#define GmReadPhantomEGSwithStGeometry_h 1

#include "GmReadPhantomEGSGeometry.hh"

#include <set>

class GmReadPhantomEGSwithStGeometry : public GmReadPhantomEGSGeometry
{
public:

  GmReadPhantomEGSwithStGeometry();
  ~GmReadPhantomEGSwithStGeometry();

  virtual void ReadPV( GmFileIn& fin );

private:

};

#endif
