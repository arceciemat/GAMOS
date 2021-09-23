// --------------------------------------------------------------
#ifndef RTExtraInfoTrackInfo_hh
#define RTExtraInfoTrackInfo_hh
#include "iaea_record.hh"

enum CutsTI{ CTI_ProdCuts = -1, CTI_MinRange = -2, CTI_RangeRejection = -3 };

#include "G4VUserTrackInformation.hh"

class RTExtraInfoTrackInfo : public G4VUserTrackInformation
{
public:

  RTExtraInfoTrackInfo( iaea_record_type *p_iaea_read ); 

  virtual ~RTExtraInfoTrackInfo();

  void Print() const {};

  long GetExtraInfoLong1() const {
    return theExtraInfoLong1;
  }
  long GetExtraInfoLong2() const {
    return theExtraInfoLong2;
  }
  float GetExtraInfoFloat1() const {
    return theExtraInfoFloat1;
  }
  float GetExtraInfoFloat2() const {
    return theExtraInfoFloat2;
  }

private:
  long theExtraInfoLong1;
  long theExtraInfoLong2;
  float theExtraInfoFloat1;
  float theExtraInfoFloat2;

};

#endif
