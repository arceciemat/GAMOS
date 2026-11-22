#include "GmVHitsIOMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmVHitsIOMgr::GmVHitsIOMgr()
{ 
  theHitsEventMgr = GmHitsEventMgr::GetInstance(); // store a copy for optimisation
  theEventTimeExtractor = 0;

  theLastHit = 0;
  bLastHitRead = FALSE;
  bFirstHitRead = FALSE;

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteAllHits()
{

  gamosSDHitMap hits = theHitsEventMgr->GetHitLists();
 
  GetFirstEventWithHit( hits );

  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh;
  //store hits to be written, so that they are later written by event ID, not by SD type
  std::multimap<G4int,GmHit*> hits2write;
  for( itehm = hits.begin(); itehm != hits.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    const hitVector* hitVector2 = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector2->begin(); iteh != hitVector2->end(); iteh++ ){
      GmHit* hit = *iteh;
      WriteHit( hit );
    } 
  }

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInCurrentEvent()
{

}


//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInEvent( G4int )
{
}


//-----------------------------------------------------------------------
G4int GmVHitsIOMgr::GetFirstEventWithHit( gamosSDHitMap hits )
{
  G4int evtID1 = INT_MAX;
  gamosSDHitMap::const_iterator ite;
  for( ite = hits.begin(); ite != hits.end(); ite++ ){
    if( (*ite).second->size() != 0 ) {
      G4int evtID = (*((*ite).second->begin()))->GetEventID();
      if( evtID < evtID1 ) evtID1 = evtID;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::GetFirstEventWithHit " << evtID1 << G4endl;
#endif

  return evtID1;

}


//-----------------------------------------------------------------------
GmVHitsIOMgr::~GmVHitsIOMgr()
{
}

