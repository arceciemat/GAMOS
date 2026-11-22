#ifndef GmRecHitBuilder1to1_HH
#define GmRecHitBuilder1to1_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"

class GmRecHitBuilder1to1 : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilder1to1();
  virtual ~GmRecHitBuilder1to1(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

private:
  G4int theNShift;
};

#endif
