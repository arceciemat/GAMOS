#include "GmRecHitBuilder1to1.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
GmRecHitBuilder1to1::GmRecHitBuilder1to1() 
{
}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilder1to1::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilder1to1::ReconstructHits " << G4endl;
#endif
  std::vector<GmRecHit*> recHits;
  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() >= theMinHitEnergy ) recHits.push_back( new GmRecHit( *iteh ) );
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilder1to1::ReconstructHits:  n RecHits = " << recHits.size() << G4endl;
#endif

  return recHits;
}
