#include "GmRecHitBuilderByDistance.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmRecHitBuilderByDistance::GmRecHitBuilderByDistance() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theHitsDist = parmgr->GetNumericValue("SD:GmRecHitBuilderByDistance:HitsDistInRecHit", 10*CLHEP::mm);
}


std::vector<GmRecHit*> GmRecHitBuilderByDistance::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByDistance::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*> recHits;
  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( itec = recHits.begin(); itec != recHits.end(); itec++ ){
      if( CheckHitInRecHit (*itec, *iteh ) ) {
	rhitFound = true;
	(*itec)->AddHit( *iteh );
	break;
      }
    }
    if( !rhitFound ) {
      recHits.push_back( new GmRecHit( *iteh ) );
    }
  }

  return recHits;
}


G4bool GmRecHitBuilderByDistance::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(infoVerb) ) G4cout << this << " ItacaRecHit::CheckHitInRecHit  rhit= " << *rhit << " hit= " << *hit << G4endl;
#endif
  if( (rhit->GetPosition() - hit->GetPosition() ).mag() < theHitsDist ){
    return true;
  } else {
    return false;
  }

}
