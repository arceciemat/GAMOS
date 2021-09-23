#ifndef GmVRecHitBuilderFromDigits_HH
#define GmVRecHitBuilderFromDigits_HH

#include "GmVRecHitBuilder.hh"
class GmDigit;

class GmVRecHitBuilderFromDigits : public GmVRecHitBuilder
{

public:
  
  GmVRecHitBuilderFromDigits(){};
  virtual ~GmVRecHitBuilderFromDigits(){};

  virtual std::vector<GmRecHit*> ReconstructDigits(const std::vector<GmDigit*>*) = 0;

};

#endif
