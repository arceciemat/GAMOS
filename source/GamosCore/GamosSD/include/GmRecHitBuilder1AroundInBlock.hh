#ifndef GmRecHitBuilder1AroundInBlockizer_HH
#define GmRecHitBuilder1AroundInBlockizer_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"

class GmRecHitBuilder1AroundInBlock : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilder1AroundInBlock();
  virtual ~GmRecHitBuilder1AroundInBlock(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

  virtual G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );

protected:
  G4double theHitsDist;
  unsigned long long theNShift;
  unsigned long long theNShiftDetector;
  G4double theMaximumEOut;
  
};

#endif
 
