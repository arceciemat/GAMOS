#include "GmHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmEventTimeExtractorFromHit.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

//-----------------------------------------------------------------------
GmHitsIOtextMgr::GmHitsIOtextMgr()
{ 
  theOutputType = "hits.text";
  Init();
  OpenFileOut();
}

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
void GmHitsIOtextMgr::WriteHit(GmHit* hit)
{
  G4String sdtype =  "/" + GmParameterMgr::GetInstance()->GetStringValue("GmHitsIOtextMgr:SDType","") + "/";
  if( sdtype != "//" && sdtype != hit->GetSDType() ) return;

  if( bDebugOut ) hit->Print( G4cout, bDebugOut );
  hit->Print( *theFileOut, bDebugOut );
}


//-----------------------------------------------------------------------
void GmHitsIOtextMgr::ReadHits()
{
  if( theFileIn == 0 ) {
    G4Exception("GmHitsIOtextMgr::ReadHits",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileIn() first ");
  }

  if( theEventTimeExtractor == 0 ) {
    theEventTimeExtractor = new GmEventTimeExtractorFromHit();
    theHitsEventMgr->SetEventTimeExtractor(theEventTimeExtractor);
  }

  if( bLastHitRead ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(warningVerb) ) G4cerr << " GmHitsIOtextMgr::ReadHits : last hit read, run is being aborted " << G4endl;
#endif
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }

  G4int evtID = 0;
  if( theLastHit != 0 ) {
    theHitsEventMgr->AddHit(theLastHit,theLastHit->GetSDType());
    evtID = theLastHit->GetEventID();
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits SetEventTime = " << theLastHit->GetTimeMin() << G4endl;
#endif
    theEventTimeExtractor->SetEventTime( theLastHit->GetTimeMin() ); // this is only approximately the event time (= initial time for first particle, but it may serve)
    theHitsEventMgr->BuildEventTime( 0 );
  }

  std::vector<G4String> wl;
  for( ;; ) {
    if( !(theFileIn->GetWordsInLine(wl)) ) {
      bLastHitRead = TRUE;
    } 
    if( wl.size() == 0) {
      bLastHitRead = TRUE;
      break;
    } else {
      GmHit* hit = new GmHit( wl );
      if( hit->GetEnergy() == 0. ) continue; // problem reading hit, E is set to 0.
      theLastHit = hit;
    }
    if( theLastHit->GetEventID() == evtID || !bFirstHitRead ) {
      evtID = theLastHit->GetEventID();
      theHitsEventMgr->AddHit(theLastHit,theLastHit->GetSDType());
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits new hit added " << theLastHit->GetEventID() << " " << theLastHit->GetSDType() << " " << theLastHit->GetDetUnitID() << G4endl;
#endif
      G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventID(evtID);
      if( !bFirstHitRead ) { // for the first hit, set the event time here
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmHitsIOtextMgr::ReadHits SetEventTime = " << theLastHit->GetTimeMin() << G4endl;
#endif
	theEventTimeExtractor->SetEventTime( theLastHit->GetTimeMin() ); // this is only approximately the event time (= initial time for first particle, but it may serve)
	theHitsEventMgr->BuildEventTime( 0 );
      }

     } else {
      break;
    }
    bFirstHitRead = TRUE;
  }

  //  G4cout << " call BuildHitsCompatibleInTime " << G4endl;
  theHitsEventMgr->BuildHitsCompatibleInTime();
  
  if( theLastHit ){
    theHitsEventMgr->AddHitToDeadTimeDetUnitList( theLastHit->GetSDType() );
  }
  
  theHitsEventMgr->DigitizeAndReconstructHits();

}


//-----------------------------------------------------------------------
GmHitsIOtextMgr::~GmHitsIOtextMgr()
{
  CloseFileIn();
  CloseFileOut();
}


