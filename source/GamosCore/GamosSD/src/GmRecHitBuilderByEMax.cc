#include "GmRecHitBuilderByEMax.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
GmRecHitBuilderByEMax::GmRecHitBuilderByEMax() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByEMax:NAncestors", 2));
  theNShift = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByEMax:NShift", 100));

  theNShift = G4int(pow(double(theNShift),NAncestors-1));
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByEMax: NShift " << theNShift << G4endl;
#endif

}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilderByEMax::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByEMax::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*> recHits;
  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  // Loop to hits 
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( itec = recHits.begin(); itec != recHits.end(); itec++ ){
      if( CheckHitInRecHit (*itec, *iteh ) ) {
	rhitFound = true;
	if( (*iteh)->GetEnergy() > (*itec)->GetHits()[0]->GetEnergy() ) {
	  (*itec)->ClearHits();
	  (*itec)->AddHit( *iteh );
	}
	break; // RecHit found
      }
    }
    if( !rhitFound ) { // new RecHit
      recHits.push_back( new GmRecHit( *iteh ) );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByEMax::ReconstructHits:  n RecHits = " << recHits.size() << G4endl;
#endif

  return recHits;
}

//------------------------------------------------------------------------
G4bool GmRecHitBuilderByEMax::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{
  
  if( int(rhit->GetHits()[0]->GetDetUnitID()/theNShift) == int(hit->GetDetUnitID()/theNShift) ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByEMax::CheckHitInRecHit merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByEMax::CheckHitInRecHit do not merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return false;
  }

}
