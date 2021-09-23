// --------------------------------------------------------------
#ifndef RTReuseAtZPlaneTrackInfo_hh
#define RTReuseAtZPlaneTrackInfo_hh

#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "globals.hh"

class RTReuseAtZPlaneTrackInfo : public GmTrackInfo
{
public:

  RTReuseAtZPlaneTrackInfo();

  virtual ~RTReuseAtZPlaneTrackInfo();

  void Print() const {};

  G4bool IsReuse() const {
    return bReuse; }

private:
  G4bool bReuse;

};

#endif
