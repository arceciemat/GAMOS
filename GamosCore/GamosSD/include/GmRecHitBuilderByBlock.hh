#ifndef GmRecHitBuilderByBlock_HH
#define GmRecHitBuilderByBlock_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"

class GmRecHitBuilderByBlock : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilderByBlock();
  virtual ~GmRecHitBuilderByBlock(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

private:
  G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );

private:
  G4int theNShift;
};

#endif
