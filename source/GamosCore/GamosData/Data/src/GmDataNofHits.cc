#include "GmDataNofHits.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"


//----------------------------------------------------------------
GmDataNofHits::GmDataNofHits()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTStep);
}

//----------------------------------------------------------------
GmDataNofHits::~GmDataNofHits()
{
}


//----------------------------------------------------------------
G4double GmDataNofHits::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return GetNofHitsThisTrack(aTrack->GetTrackID());
}

//----------------------------------------------------------------
G4double GmDataNofHits::GetValueFromEvent( const G4Event* , G4int )
{
  return GetNofHits();
}

//----------------------------------------------------------------
G4int GmDataNofHits::GetNofHits()
{
  gamosSDHitMap hitlists = GmHitsEventMgr::GetInstance()->GetHitLists();
 
  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh,iteh2;
  std::map<G4int,G4int> hitsInSDType; 
  G4int nh = 0;
  //---- Loop to hits in this event
  //  hitsInEvent.clear();
  for( itehm = hitlists.begin(); itehm != hitlists.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    //    G4int nht;
    const hitVector* hitVector2 = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector2->begin(); iteh != hitVector2->end(); iteh++, nh++ ){
      //      GmHit* hit = *iteh;
      //     nht = GetHitTypeInt( hit );
      //      hitsInSDType[nht]++; //count hits of each SD type      
    }
  }

  return nh;

  /*  //----- Fill histo of number of hits per type
  std::map<G4int,G4int>::const_iterator itet;
  for( itet = hitsInSDType.begin(); itet != hitsInSDType.end(); itet++, nh++ ){
    theAnaMgr->GetHisto1(201000+nh*100+1)->Fill( float((*itet).second) );
    }*/

}

//----------------------------------------------------------------
G4int GmDataNofHits::GetNofHitsThisTrack(G4int theTrackID )
{
  gamosSDHitMap hitlists = GmHitsEventMgr::GetInstance()->GetHitLists();
 
  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh,iteh2;
  std::map<G4int,G4int> hitsInSDType; 
  G4int nh = 0;
  //---- Loop to hits in this event
  //  hitsInEvent.clear();
  for( itehm = hitlists.begin(); itehm != hitlists.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    //    G4int nht;
    const hitVector* hitVector2 = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector2->begin(); iteh != hitVector2->end(); iteh++ ){
      std::set<G4int> trackIDs = (*iteh)->GetTrackIDs();
      if( trackIDs.find(theTrackID) != trackIDs.end() ) nh++;
    }
  }

  return nh;

}
