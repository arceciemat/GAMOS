#ifndef GmRecHitBuilderSmearDOIByEMax_HH
#define GmRecHitBuilderSmearDOIByEMax_HH

#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"
#include "globals.hh"
class GmHit;

class GmRecHitBuilderSmearDOIByEMax : public GmVRecHitBuilderFromHits
{

public:
  
  GmRecHitBuilderSmearDOIByEMax();
  virtual ~GmRecHitBuilderSmearDOIByEMax(){};

  virtual std::vector<GmRecHit*> ReconstructHits(const std::vector<GmHit*>*);

private:
  G4bool CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit );
  
private:
  G4int theNShift;
  G4double theDOIResol;
  
  void SmearROI(GmHit *hit);
  
};

#endif
