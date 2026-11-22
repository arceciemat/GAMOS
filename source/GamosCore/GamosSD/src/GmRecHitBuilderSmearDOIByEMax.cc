#include "GmRecHitBuilderSmearDOIByEMax.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "CLHEP/Random/RandGauss.h"

//------------------------------------------------------------------------
GmRecHitBuilderSmearDOIByEMax::GmRecHitBuilderSmearDOIByEMax() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderSmearDOIByEMax:NAncestors", 2));
  theNShift = G4int(parmgr->GetNumericValue("SD:GmRecHitBuilderSmearDOIByEMax:NShift", 100));

  theNShift = G4int(pow(double(theNShift),NAncestors-1));
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderSmearDOIByEMax: NShift " << theNShift << G4endl;
#endif

  theDOIResol = GmParameterMgr::GetInstance()->GetNumericValue("SD:GmRecHitBuilderSmearDOIByEMax:DOIResol",0.); 
}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilderSmearDOIByEMax::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderSmearDOIByEMax::ReconstructHits " << G4endl;
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
	  SmearROI(*iteh);
	  (*itec)->AddHit( *iteh );
	}
	break; // RecHit found
      }
    }
    if( !rhitFound ) { // new RecHit
      SmearROI(*iteh);
      recHits.push_back( new GmRecHit( *iteh ) );
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilderSmearDOIByEMax::ReconstructHits:  n RecHits = " << recHits.size() << G4endl;
#endif

  return recHits;
}

//------------------------------------------------------------------------
G4bool GmRecHitBuilderSmearDOIByEMax::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{
  
  if( int(rhit->GetHits()[0]->GetDetUnitID()/theNShift) == int(hit->GetDetUnitID()/theNShift) ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderSmearDOIByEMax::CheckHitInRecHit merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilderSmearDOIByEMax::CheckHitInRecHit do not merge hits into rhit : " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return false;
  }

}

//------------------------------------------------------------------------
void GmRecHitBuilderSmearDOIByEMax::SmearROI(GmHit *hit)
{

  G4ThreeVector globalPos = hit->GetPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( globalPos, touch, false );
  
  G4AffineTransform transInv = touch->GetHistory()->GetTopTransform().Inverse();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform();
  delete touch;
  
  G4ThreeVector localPos = trans.TransformPoint( globalPos );
  //  G4cout << " GmRecHitBuilderSmearDOIByEMax::SmearROI globalPos=" << globalPos << " localPos=" << localPos << G4endl; //GDEB
  G4double ran = CLHEP::RandGauss::shoot(0., theDOIResol );
  localPos = G4ThreeVector(0.,0.,localPos.z()+ran);
  //  G4cout << " GmRecHitBuilderSmearDOIByEMax::SmearROI localPos_Smeared=" << localPos << " ran= " << ran << " theDOIResol=" << theDOIResol << G4endl; //GDEB
  globalPos = transInv.TransformPoint( localPos );
  //  G4cout << " GmRecHitBuilderSmearDOIByEMax::SmearROI globalPos_FINAL=" << globalPos << G4endl; //GDEB

  hit->SetPosition( globalPos );

  
}
