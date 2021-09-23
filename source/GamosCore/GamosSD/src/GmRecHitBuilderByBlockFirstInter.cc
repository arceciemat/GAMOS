#include "GmRecHitBuilderByBlockFirstInter.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
GmRecHitBuilderByBlockFirstInter::GmRecHitBuilderByBlockFirstInter() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByBlockFirstInter:NAncestors", 2));
  theNShift = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderByBlockFirstInter:NShift", 100));

  theNShift = G4int(pow(double(theNShift),NAncestors-1));
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlockFirstInter: NShift " << theNShift << G4endl;
#endif
}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilderByBlockFirstInter::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlockFirstInter::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*> recHits;
  std::vector<GmRecHit*>::iterator iterh;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    if( (*iteh)->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( iterh = recHits.begin(); iterh != recHits.end(); iterh++ ){
      if( CheckHitInRecHit (*iterh, *iteh ) ) {
	rhitFound = true;
	AddHit(*iterh, *iteh );
	break;
      }
    }
    if( !rhitFound ) {
      recHits.push_back( new GmRecHit( *iteh ) );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderByBlockFirstInter::ReconstructHits:  n RecHits = " << recHits.size() << G4endl;
#endif

  return recHits;
}


//------------------------------------------------------------------------
G4bool GmRecHitBuilderByBlockFirstInter::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{
  
  if( int(rhit->GetHits()[0]->GetDetUnitID()/theNShift) == int(hit->GetDetUnitID()/theNShift) ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByBlockFirstInter::CheckHitInRecHit merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderByBlockFirstInter::CheckHitInRecHit do not merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return false;
  }

}


//------------------------------------------------------------------------
void GmRecHitBuilderByBlockFirstInter::AddHit( GmRecHit* rhit, GmHit* hit )
{

  rhit->AddHit(hit);
  // loop to hits and take position of first hit
  //--- position is barycentre of hits
  std::vector<GmHit*>::const_iterator iteh;
  G4double minTime = DBL_MAX;
  std::vector<GmHit*> theHits = rhit->GetHits();
  for( iteh = theHits.begin(); iteh != theHits.end(); iteh++ ) {
    GmHit* hit2 = *iteh;
    if( hit2->GetTime() < minTime ) {
      minTime = hit2->GetTime();
      rhit->SetPosition(hit2->GetPosition());
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmRecHitBuilderByBlockFirstInter::AddHit  position= " << rhit->GetPosition()<< G4endl;
#endif
}

