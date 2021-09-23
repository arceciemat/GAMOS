#ifndef GmRecHitBuilderByEMax_HH
#define GmRecHitBuilderByEMax_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"
#include "globals.hh"
class GmHit;

class GmRecHitBuilderByEMax : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilderByEMax();
  virtual ~GmRecHitBuilderByEMax(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

private:
  G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );
  
private:
  G4int theNShift;

};

#endif
