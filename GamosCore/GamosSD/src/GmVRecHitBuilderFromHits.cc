#include "GamosCore/GamosSD/include/GmVRecHitBuilderFromHits.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"

void GmVRecHitBuilderFromHits::CleanRecHits()
{
  std::vector<GmRecHit*>::iterator ite;
  for(ite = theRecHits.begin(); ite != theRecHits.end(); ite++ ){
    delete *ite;
  }

  theRecHits.clear();
}  

void GmVRecHitBuilderFromHits::DeleteHit( GmRecHit* rhit )
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
