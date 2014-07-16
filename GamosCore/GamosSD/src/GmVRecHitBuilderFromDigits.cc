#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromDigits.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"

void GmVRecHitBuilderFromDigits::CleanRecHits()
{
  std::vector<GmRecHit*>::iterator ite;
  for(ite = theRecHits.begin(); ite != theRecHits.end(); ite++ ){
    delete *ite;
  }

  theRecHits.clear();
}  

void GmVRecHitBuilderFromDigits::DeleteHit( GmRecHit* rhit )
{
  std::vector<GmRecHit*>::iterator ite;
  for(ite = theRecHits.begin(); ite != theRecHits.end(); ite++ ){
    if( *ite  == rhit ) {
      delete *ite;
      theRecHits.erase( ite );
      break;
    }
  }
}
