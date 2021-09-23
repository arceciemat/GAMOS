#ifndef GmVRecHitBuilderFromHits_HH
#define GmVRecHitBuilderFromHits_HH

#include "GmVRecHitBuilder.hh"
class GmHit;

class GmVRecHitBuilderFromHits : public GmVRecHitBuilder
{

public:
  
  GmVRecHitBuilderFromHits(){};
  virtual ~GmVRecHitBuilderFromHits(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*) = 0;
};

#endif
