#ifndef GmRecHitPair_HH
#define GmRecHitPair_HH

#include "globals.hh"
#include <vector>
class GmRecHit;


class GmRecHitPair 
{

public:
  GmRecHitPair(){};
  
  GmRecHitPair( GmRecHit* rhit1, GmRecHit* rhit2 );
  virtual ~GmRecHitPair(){};

  G4double GetEnergy() const;
  GmRecHit* recHit1() const { return theRecHit1; }
  GmRecHit* recHit2() const { return theRecHit2; }
private:
  GmRecHit *theRecHit1, *theRecHit2;
};

typedef std::vector<GmRecHitPair> GmRecHitPairList;

#endif
