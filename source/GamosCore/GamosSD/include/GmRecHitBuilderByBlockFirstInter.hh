#ifndef GmRecHitBuilderByBlockFirstInter_HH
#define GmRecHitBuilderByBlockFirstInter_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"

class GmRecHitBuilderByBlockFirstInter : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilderByBlockFirstInter();
  virtual ~GmRecHitBuilderByBlockFirstInter(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

private:
  G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );
  void AddHit( GmRecHit* rhit, GmHit* hit );

private:
  G4int theNShift;
};

#endif
