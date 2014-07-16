#ifndef GmRecHitBuilderByDistanceizer_HH
#define GmRecHitBuilderByDistanceizer_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"

class GmRecHitBuilderByDistance : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilderByDistance();
  virtual ~GmRecHitBuilderByDistance(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

  virtual G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );

private:
  G4double theHitsDist;

};

#endif
 
