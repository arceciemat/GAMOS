#include "GmRecHitBuilder1AroundInBlock.hh"
#include "GmSDVerbosity.hh"
#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitList.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------
GmRecHitBuilder1AroundInBlock::GmRecHitBuilder1AroundInBlock() 
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theHitsDist = parmgr->GetNumericValue("SD:GmRecHitBuilder1AroundInBlock:HitsDistInRecHit", 10*CLHEP::mm);

  theNShift = G4int(parmgr->GetNumericValue("SD:DetUnitID:NShift", 100));
  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:DetUnitID:NAncestors", 2));
  G4int NAncestorsBlock = G4int(parmgr->GetNumericValue("SD:DetUnitID:NAncestorsBlock", 1));
  theNShiftDetector = G4int(parmgr->GetNumericValue("SD:DGmRecHitBuilder1AroundInBlock:NDetector", 1)); // only one rechit per detector
  theNShiftDetector = pow(double(theNShift),NAncestors-theNShiftDetector);
  theNShift = pow(double(theNShift),NAncestors-NAncestorsBlock);

  theMaximumEOut = parmgr->GetNumericValue("SD:GmRecHitBuilder1AroundInBlock:MaximumEnergyOut", 0.05);

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock: Distance " << theHitsDist << " NShift " << theNShift << " theNShiftDetector " << theNShiftDetector<< " theMaximumEOut " << theMaximumEOut << G4endl;
#endif
}


//------------------------------------------------------
std::vector<GmRecHit*> GmRecHitBuilder1AroundInBlock::ReconstructHits(const std::vector<GmHit*>* hvect )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock::ReconstructHits " << G4endl;
#endif

  std::vector<GmRecHit*> recHitsTEMP;
  std::map<  unsigned long long, std::vector<GmRecHit*> > recHitsMap; // list of recHits for each detector
  std::vector<GmRecHit*>::const_iterator itec;
  hitVector::const_iterator iteh;
  //assume hits are ordered by energy
  for( iteh = hvect->begin(); iteh != hvect->end(); iteh++ ){
    GmHit* hit = *iteh;
    if( hit->GetEnergy() < theMinHitEnergy ) continue;
    G4bool rhitFound = false;
    //--- Check if hit belongs to one of the existing rechits
    for( itec = recHitsTEMP.begin(); itec != recHitsTEMP.end(); itec++ ){
      if( CheckHitInRecHit (*itec, hit ) ) {
	rhitFound = true;
	(*itec)->AddHit( hit );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock add hit to RecHit " << hit->GetDetUnitID() << G4endl;
#endif
	break;
      }
    }
    if( !rhitFound ) {
      unsigned long long ndetector = hit->GetDetUnitID()/theNShiftDetector;
      GmRecHit* newRecHit = new GmRecHit( hit );
      recHitsMap[ndetector].push_back( newRecHit );
      recHitsTEMP.push_back( newRecHit );
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(testVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock new RecHit " << hit->GetDetUnitID() << " " << ndetector << " " << recHitsMap[ndetector].size() << G4endl;
#endif
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(testVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock NRecHits= " << recHitsTEMP.size() << " NHits= " << hvect->size() << G4endl;
#endif
  
  //--- NO MORE THAN ONE REC HIT IN SAME BLOCK
  for( std::map<  unsigned long long,std::vector<GmRecHit*> >::iterator itehm = recHitsMap.begin(); itehm != recHitsMap.end(); itehm++ ) {
    std::vector<GmRecHit*> recHitsV = itehm->second;
    G4int nBigE = 0;
    for( itec = recHitsV.begin(); itec != recHitsV.end(); itec++ ) {
      if( (*itec)->GetEnergy() > theMaximumEOut ) {
	nBigE ++;
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(testVerb) ) G4cout <<itehm->first << " GmRecHitBuilder1AroundInBlock nBigE " << nBigE << " nRecHits " << recHitsV.size() << " in detector " << itehm->first << G4endl;
#endif
    if( nBigE != 1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(testVerb) ) G4cout <<  recHitsTEMP.size() << " GmRecHitBuilder1AroundInBlock not 1 RecHits in detector " << nBigE << " Detector " << itehm->first << " !! DELETING ALL " << G4endl;
#endif
      recHitsTEMP.clear();

      /*      for( itec = recHitsTEMP.begin(); itec != recHitsTEMP.end(); itec++ ){
	delete *itec;
      }
      return recHitsTEMP;
      */
      
      break;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(testVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock FINAL NRecHits= " << recHitsTEMP.size() << " before= " << recHitsTEMP.size() << " NHits= " << hvect->size() << G4endl;
#endif

  //--- MERGE INTO ONE RECHIT THE RECHITS IN THE SAME BLOCK
  //--- SET RECHIT POSITION AS THAT OF THE BIGGEST HIT
  std::vector<GmRecHit*> recHits;
  for( std::map<  unsigned long long,std::vector<GmRecHit*> >::iterator itehm = recHitsMap.begin(); itehm != recHitsMap.end(); itehm++ ) {
    std::vector<GmRecHit*> recHitsV = itehm->second;
    G4ThreeVector rhPos;
    G4double maxEner = 0.;
    GmRecHit* newRecHit = 0;
    for( itec = recHitsV.begin(); itec != recHitsV.end(); itec++ ) {
      if( itec == recHitsV.begin() ) {
	newRecHit = new GmRecHit( *itec );
	recHits.push_back(newRecHit);
	rhPos = (*itec)->GetPosition();
      } else {
	(*newRecHit) += *(*itec);
	if( (*itec)->GetEnergy() > maxEner ) {
	  maxEner = (*itec)->GetEnergy();
	  rhPos = (*itec)->GetPosition();
	}
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << " GmRecHitBuilder1AroundInBlock add Rechit to RechHit " << (*itec)->GetEnergy() << " " << (*itec)->GetPosition() << G4endl;
#endif
      }
    }
    newRecHit->SetPosition(rhPos);
  }

  for( itec = recHitsTEMP.begin(); itec != recHitsTEMP.end(); itec++ ){
    delete *itec;
  }
  
  return recHits;
}


//------------------------------------------------------
G4bool GmRecHitBuilder1AroundInBlock::CheckHitInRecHit( const GmRecHit* rhit, const GmHit* hit )
{

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(infoVerb) ) G4cout << this << " ItacaRecHit::CheckHitInRecHit  rhit= " << *rhit << " hit= " << *hit << G4endl;
#endif
  //  G4cout << " GmRecHitBuilder1AroundInBlock::CheckHitInRecHit( DUID " << ( (unsigned long long)(rhit->GetHits()[0]->GetDetUnitID()/theNShift) == (unsigned long long)(hit->GetDetUnitID()/theNShift) ) << " "<<  (unsigned long long)(rhit->GetHits()[0]->GetDetUnitID()/theNShift) << " =? " <<  (unsigned long long)(hit->GetDetUnitID()/theNShift) << " " << rhit->GetHits()[0]->GetDetUnitID()  << " " << hit->GetDetUnitID() << " dist " << (rhit->GetPosition() - hit->GetPosition() ).mag() << " " << rhit->GetPosition() << " - " << hit->GetPosition() << G4endl; //GDEB
  if(  (unsigned long long)(rhit->GetHits()[0]->GetDetUnitID()/theNShift) ==  (unsigned long long)(hit->GetDetUnitID()/theNShift) &&
      (rhit->GetPosition() - hit->GetPosition() ).mag() < theHitsDist ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilder1AroundInBlock::CheckHitInRecHit merge hits into rhit : DIST = " << (rhit->GetPosition() - hit->GetPosition() ).mag() << " " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << this << " GmRecHitBuilder1AroundInBlock::CheckHitInRecHit DO NOT merge hits into rhit : DIST = " << (rhit->GetPosition() - hit->GetPosition() ).mag() << " " << rhit->GetHits()[0]->GetDetUnitID() << " = " << hit->GetDetUnitID() << " NShift " << theNShift << G4endl;
#endif
    return false;
  }

}
