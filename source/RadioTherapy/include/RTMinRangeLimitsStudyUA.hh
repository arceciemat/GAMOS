#ifndef RTMinRangeLimitsStudyUA_hh
#define RTMinRangeLimitsStudyUA_hh

#include "GamosCore/GamosPhysics/Cuts/include/GmMinRangeLimitsStudyUA.hh"

class RTMinRangeLimitsStudyUA : public GmMinRangeLimitsStudyUA
{
public:
  RTMinRangeLimitsStudyUA();
  ~RTMinRangeLimitsStudyUA(){}; 
  virtual void BuildFilter();

protected:

};

#endif
