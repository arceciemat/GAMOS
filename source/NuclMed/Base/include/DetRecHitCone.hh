#ifndef DetRecHitCone_HH
#define DetRecHitCone_HH
#include "GamosCore/GamosUtils/include/GmCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"

class DetRecHitCone : public GmCone
{

public:  // with description

  // Constructor & destructor.
  DetRecHitCone(const GmRecHit* rhitTracker, GmRecHit* rhitCalor );
  ~DetRecHitCone(){};

private:
 
};

#endif
