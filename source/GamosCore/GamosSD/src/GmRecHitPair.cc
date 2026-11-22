#include "GmRecHitPair.hh"
#include "GmRecHit.hh"


//----------------------------------------------------------------------
GmRecHitPair::GmRecHitPair( GmRecHit* rhit1, GmRecHit* rhit2 )
{
  if( rhit1 == 0 ) {
    G4Exception("GmRecHitPair::GmRecHitPair",
		"Error",
		FatalException,
		"First recHit is 0 ");
  }
  theRecHit1 = rhit1;
  theRecHit2 = rhit2;
}


//----------------------------------------------------------------------
G4double GmRecHitPair::GetEnergy() const
{
  if( theRecHit2 == 0 ){
    return theRecHit1->GetEnergy();
  }else {
    return theRecHit1->GetEnergy() + theRecHit2->GetEnergy();
  }
}
