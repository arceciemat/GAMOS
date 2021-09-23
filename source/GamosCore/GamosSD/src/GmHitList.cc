#include "GamosCore/GamosSD/include/GmHitList.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosSD/include/GmDeadTimeDUListByCrystal.hh"
#include "GamosCore/GamosSD/include/GmDeadTimeDUListByBlock.hh"

#include "G4RunManager.hh"
#include "CLHEP/Random/RandGauss.h"

//----------------------------------------------------------------------
GmHitList::GmHitList( G4String& sdtypG4 ): theSDType( sdtypG4 )
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4String sdtype = GetSDType();
  sdtype = sdtype.substr(1,sdtypG4.size()-2);
  
  G4String deadtimeType = parmgr->GetStringValue("SD:DeadTimeType:"+sdtype,"byBlock");

  if( deadtimeType == "byCrystal" ){
    theDeadTimeDetUnitList = new GmDeadTimeDUListByCrystal(sdtype);
  }else if( deadtimeType == "byBlock" ){
    theDeadTimeDetUnitList = new GmDeadTimeDUListByBlock(sdtype);
  } else {
    G4Exception("GmHitList::GmHitList",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("DeadDetUnitList has invalid type " + deadtimeType).c_str());
  }

  theMeasuringTime = parmgr->GetNumericValue("SD:MeasuringTime:"+sdtype, 0.*CLHEP::ns); // save it here for efficiency
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmHitList sdtype " << sdtype << " theMeasuringTime " << theMeasuringTime << G4endl;
#endif

  theDeadTime = parmgr->GetNumericValue("SD:DeadTime:"+sdtype, 0.*CLHEP::ns); // save it here for efficiency
#ifndef GAMOS_NO_VERBOSE
if( SDVerb(infoVerb) ) G4cout << "GmHitList sdtype " << sdtype << " theDeadTime " << theDeadTime << G4endl;
#endif
  
  //----- When hits are written to be read back, if the measuring time is smaller than the dead time, at normal run a previous event may have deleted one hit because a hit was created and the SD is dead, while when reading hits, if this previous event is not written, the hit has not been deleted. 
  //  if( theDeadTime > theMeasuringTime ) theMeasuringTime = theDeadTime;

  bParalyzable= G4bool(parmgr->GetNumericValue("SD:DeadTimeParalyzable:"+sdtype, 0)); 
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmHitList sdtype " << sdtype << " Paralyzable " << bParalyzable << G4endl;
#endif


  G4String measType = parmgr->GetStringValue("SD:MeasuringType:"+sdtype,"TriggerIndependent");
  if( measType == "TriggerGlobal" ){
    theMeasuringType = bMTTriggerGlobal;
  } else if( measType == "TriggerIndependent"  || 
	     measType == "Trigger" ){
    theMeasuringType = bMTTriggerIndependent;
    theNShift = G4int(parmgr->GetNumericValue("SD:DetUnitID:NShift:"+sdtype, 100));
    theNTrigAncestors = G4int(parmgr->GetNumericValue("SD:Trigger:NAncestors:"+sdtype, 3));
  } else if( measType == "Backwards" ){
    theMeasuringType = bMTBackwards;
  } else if( measType == "Interval" ){
    theMeasuringType = bMTInterval;
  } else {
    G4Exception("GmHitList::GmHitList",
		G4String("Wrong type of measuring type for SDtype "+sdtype).c_str(),
		FatalErrorInArgument,
		G4String("Types can be: TriggerGlobal / TriggerIndependent / Interval / Backwards, it is = " + measType).c_str());
  }
  
  theCurrentIntervalStartTime = 0.;
  thePreviousEventIntervalTime = -theMeasuringTime;

  theRunManager = G4RunManager::GetRunManager();
}


//----------------------------------------------------------------------
GmHitList::~GmHitList()
{
}


//----------------------------------------------------------------------
void GmHitList::AddHit( GmHit* hit )
{
  //----- Check hit is of the same SD type as the first one
  if( size() != 0 ){
    if( hit->GetSDType() != theSDType ){
      G4Exception("GmHitList::AddHit",
		  "Error",
		  FatalException,
		  G4String("Not equal SD type " + theSDType + " <> " + hit->GetSDType()).c_str());
    }
  }
  
  push_back( hit );

}


//------------------------------------------------------------------------
void GmHitList::AddHitToDeadTimeDetUnitList()
{
  //t   if( theDeadTime == 0. ) return;

  //-  G4cout << " AddHitToDeadTimeDetUnitList " << size() << G4endl;
  //----- Add hit to list of dead time hits
  GmHitList::iterator iteh;
  for( iteh = begin(); iteh != end(); iteh++ ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << " GmHitList::AddHitToDeadTimeDetUnitList " 
	   << " bParalyzable= " << bParalyzable 
	   << " DeadTimeFound= " << (*iteh)->DeadTimeFound() 
	   << " SDTYPE= " <<(*iteh)->GetSDType() 
	   << " DU= " << (*iteh)->GetDetUnitID() << G4endl;
#endif
    if( bParalyzable || !(*iteh)->DeadTimeFound() ) theDeadTimeDetUnitList->AddDetUnit( *iteh );

  }
}


//------------------------------------------------------------------------
void GmHitList::CleanHits( G4double tim )
{
  if( tim == -1 || theMeasuringTime == 0. ) {
    GmHitList::iterator iteh;
    //--- Loop to hits
    for( iteh = begin(); iteh != end(); iteh++ ){
      delete *iteh;
    }
    clear();
  } else {
    if( theMeasuringType == bMTBackwards ) {
      CleanHitsBefore( tim - 2* theMeasuringTime );
    } else if( theMeasuringType == bMTInterval ) {
      CleanHitsBefore( theCurrentIntervalStartTime );
    } else if( theMeasuringType == bMTTriggerGlobal ) {
      CleanHitsBefore( theCurrentIntervalStartTime );
    } else if( theMeasuringType == bMTTriggerIndependent ) {
      std::map<unsigned long long, G4double>::const_iterator itetm;
      for( itetm = theCurrentIntervalStartTimes.begin(); itetm != theCurrentIntervalStartTimes.end(); itetm++ ){
	CleanHitsBefore( (*itetm).second, (*itetm).first );
      }
    }
  }
 
  theHitsCompatibleInTime.clear();

}

//------------------------------------------------------------------------
void GmHitList::CleanHitsBefore( G4double tim )
{
  // clear all the hits with time less than tim
  std::vector<GmHitList::iterator> vitehits;
  GmHitList::iterator iteh;
  //--- Loop to hits
  for( iteh = begin(); iteh != end(); iteh++ ){
    if( (*iteh)->GetTime() < tim ){
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  yes deleting hit, time= " << (*iteh)->GetTime() << " is < " << tim << " theMeasuringTime "<< theMeasuringTime << G4endl;
#endif
      
      //	delete *iteh;
      vitehits.push_back( iteh );
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  not deleting hit, time= " << (*iteh)->GetTime() << " is >= " << tim << " theMeasuringTime "<< theMeasuringTime << G4endl;
#endif
    }
  }
  
  std::vector<GmHitList::iterator>::reverse_iterator iteitev;
  for( iteitev = vitehits.rbegin(); iteitev != vitehits.rend(); iteitev++ ){
    //-      G4cout << this << " GmHitList deleting iterator " << size() << G4endl; 
    DeleteHit( *iteitev );
  }
  //     G4cout << " looped hits " << size() << G4endl;
  for( iteh = begin(); iteh != end(); iteh++ ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  not deleted hit " << (*iteh)->GetTime() << G4endl;
#endif
  }
}

//------------------------------------------------------------------------
void GmHitList::CleanHitsBefore( G4double tim, unsigned long long trigID )
{
  // clear all the hits with time less than tim
  std::vector<GmHitList::iterator> vitehits;
  GmHitList::iterator iteh;
  //--- Loop to hits
  for( iteh = begin(); iteh != end(); iteh++ ){
    unsigned long long hitTrigID = (*iteh)->GetDetUnitID() / G4int(pow(double(theNShift),int(theNTrigAncestors)));
    if( hitTrigID != trigID ) continue;

    if( (*iteh)->GetTime() < tim ){
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  yes deleting hit " << (*iteh)->GetTime() << " is < " << tim << " theMeasuringTime "<< theMeasuringTime << G4endl;
#endif
      
      //	delete *iteh;
      vitehits.push_back( iteh );
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  not deleting hit, time= " << (*iteh)->GetTime() << " is >= " << tim << " theMeasuringTime "<< theMeasuringTime << G4endl;
#endif
    }
  }
  
  std::vector<GmHitList::iterator>::reverse_iterator iteitev;
  for( iteitev = vitehits.rbegin(); iteitev != vitehits.rend(); iteitev++ ){
    //-      G4cout << this << " GmHitList deleting iterator " << size() << G4endl; 
    DeleteHit( *iteitev );
  }
  //     G4cout << " looped hits " << size() << G4endl;
  for( iteh = begin(); iteh != end(); iteh++ ){
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanHits  not deleted hit " << (*iteh)->GetTime() << G4endl;
#endif
  }
}


//----------------------------------------------------------------------
void GmHitList::CleanDeadTimeDetUnitList(G4double time)
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmHitList::CleanDeadTimeDetUnitList event time= " << time << " deadtime interval= " << theDeadTime << G4endl;
#endif
  theDeadTimeDetUnitList->Clean( time - theDeadTime );
}


//----------------------------------------------------------------------
void GmHitList::BuildHitsCompatibleInTime( G4double currentTime )
{
  if( currentTime == -1. || currentTime == 0. || theMeasuringTime == 0. ) {
    BuildHitsAll( currentTime );
    return;
  }

  if( theMeasuringType == bMTBackwards ) {
    BuildHitsCompatibleInTimeBackwards( currentTime );
  } else if( theMeasuringType == bMTInterval ) {
    BuildHitsCompatibleInTimeInterval( currentTime );
  } else if( theMeasuringType == bMTTriggerGlobal ) {
    BuildHitsCompatibleInTimeTriggerGlobal( currentTime );
  } else if( theMeasuringType == bMTTriggerIndependent ) {
    BuildHitsCompatibleInTimeTriggerIndependent( currentTime );
  }

}

//----------------------------------------------------------------------
void GmHitList::BuildHitsAll( G4double currentTime )
{
  iterator ite;
  for( ite = begin(); ite != end(); ite++ ){
    G4bool bOK = true;
    G4double hitTime = (*ite)->GetTime();
    G4double lowestTime = currentTime - theMeasuringTime;
    G4bool bDeadDU = theDeadTimeDetUnitList->FindDetUnit(*ite,theDeadTime); 
    if( bDeadDU ) {
      bOK = false;
      (*ite)->SetDeadTimeFound(true);
    }
    if( hitTime < lowestTime ) {
      bOK = false;
    }
    
    if( bOK == true) {
      theHitsCompatibleInTime.push_back( *ite );
    }
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTime added hit, because all are added " << hitTime << G4endl;
#endif
    
  }
}

//----------------------------------------------------------------------
void GmHitList::BuildHitsCompatibleInTimeBackwards( G4double currentTime )
{

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleWithTime  event Time= " << currentTime << G4endl;
#endif
  //----- list hits whose time is close to the current event time (closer than theMeasuringtTime) and are not in det units with a recent hit (more recent than theDeadTime )
  GmHitList::iterator ite;
  G4double lowestTime = currentTime - theMeasuringTime;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeBackwards  looking between times " << lowestTime << " - " << currentTime << G4endl;
#endif
  for( ite = begin(); ite != end(); ite++ ){
    G4bool bOK = true;
    G4double hitTime = (*ite)->GetTime();
    G4bool bDeadDU = theDeadTimeDetUnitList->FindDetUnit(*ite,theDeadTime); 
    if( bDeadDU ) {
      bOK = false;
      (*ite)->SetDeadTimeFound(true);
    }
    if( hitTime < lowestTime ) {
      bOK = false;
    }
    if( bOK == true) {
      theHitsCompatibleInTime.push_back( *ite );
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeBackwards  hit OK. "; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeBackwards  hit NOT OK. ";	  
#endif
    }
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) )
      G4cout << " hit time= " << hitTime 
	     << " >? lowestTime= " << lowestTime 
	     << " is DeadTimeDetUnit= " << bDeadDU 
	     << "  N hitscomp= " << theHitsCompatibleInTime.size()
	     << " HIT evtID= " << (*ite)->GetEventID() 
	     << " DU= " << (*ite)->GetDetUnitID() 
	     << " E= " << (*ite)->GetEnergy() << G4endl;
#endif
    
  }
}


//----------------------------------------------------------------------
void GmHitList::BuildHitsCompatibleInTimeInterval( G4double currentTime )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeInterval  currentTime= " << currentTime << " theCurrentIntervalStartTime= " << theCurrentIntervalStartTime << G4endl;
#endif
  //----- list hits whose time is in the current measuring interval and are not in det units with a recent hit (more recent than theDeadTime )
  GmHitList::iterator ite;

  //----- Select which time interval is being used
  if( currentTime > thePreviousEventIntervalTime + theMeasuringTime ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeInterval  change theCurrentIntervalStartTime= " <<  thePreviousEventIntervalTime + theMeasuringTime << " old " << theCurrentIntervalStartTime << G4endl;
#endif
    theCurrentIntervalStartTime = thePreviousEventIntervalTime + theMeasuringTime;
  }
  thePreviousEventIntervalTime = G4int(currentTime/theMeasuringTime)*theMeasuringTime;

  G4double lowestTime = theCurrentIntervalStartTime - theMeasuringTime;
  G4double highestTime = theCurrentIntervalStartTime;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeInterval  looking between times " << lowestTime << " - " << highestTime << G4endl;
#endif
  for( ite = begin(); ite != end(); ite++ ){
    G4bool bOK = true;
    G4double hitTime = (*ite)->GetTime();
    G4bool bDeadDU = theDeadTimeDetUnitList->FindDetUnit(*ite,theDeadTime); 
    if( bDeadDU ) {
      bOK = false;
      (*ite)->SetDeadTimeFound(true);
    }
    if( hitTime < lowestTime || hitTime >= highestTime ) {
      bOK = false;
    }
    if( bOK == true) {
      theHitsCompatibleInTime.push_back( *ite );
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeInterval  hit OK. "; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeInterval  hit NOT OK. ";	  
#endif
    }
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) 
      G4cout << " hit time= " << hitTime 
	     << " <? lowestTime= " << lowestTime 
	     << " >=? highestTime= " << highestTime 
	     << " is DeadTimeDetUnit= " << bDeadDU 
	     << "  N hitscomp= " << theHitsCompatibleInTime.size()
	     << " HIT evtID= " << (*ite)->GetEventID() 
	       << " DU= " << (*ite)->GetDetUnitID() 
	     << " E= " << (*ite)->GetEnergy() << G4endl;
#endif
    
  }
}


//----------------------------------------------------------------------
void GmHitList::BuildHitsCompatibleInTimeTriggerGlobal( G4double currentTime )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) 
    G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerGlobal  currentTime= " << currentTime 
	   << " theCurrentIntervalStartTime= " << theCurrentIntervalStartTime
	   << " thePreviousEventIntervalTimes= " << thePreviousEventIntervalTime << G4endl;
#endif
  //----- list hits whose time is in the current measuring interval and are not in det units with a recent hit (more recent than theDeadTime )
  //----- Select which time interval is being used
  if( currentTime > thePreviousEventIntervalTime + theMeasuringTime ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerGlobal  change theCurrentIntervalStartTime= " <<  thePreviousEventIntervalTime + theMeasuringTime 
				   << " old " << theCurrentIntervalStartTime 
				   << " thePreviousEventIntervalTime " << currentTime-1.E-6*theMeasuringTime << G4endl;

#endif
    theCurrentIntervalStartTime = thePreviousEventIntervalTime + theMeasuringTime;
    thePreviousEventIntervalTime = currentTime-1.E-6*theMeasuringTime; // to avoid precision problems substract 1.E-6 ns
    //      currentTime;
  }

  G4double lowestTime = theCurrentIntervalStartTime - theMeasuringTime;
  G4double highestTime = theCurrentIntervalStartTime;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerGlobal  looking between times " << lowestTime << " - " << highestTime << G4endl;
#endif
  GmHitList::iterator iteh;
  for( iteh = begin(); iteh != end(); iteh++ ){
    G4bool bOK = true;
    G4double hitTime = (*iteh)->GetTime();
    G4bool bDeadDU = theDeadTimeDetUnitList->FindDetUnit(*iteh,theDeadTime); 
    if( bDeadDU ) {
      bOK = false;
      (*iteh)->SetDeadTimeFound(true);
    }
    if( hitTime < lowestTime || hitTime >= highestTime ) {
      bOK = false;
    }
    if( bOK == true) {
      theHitsCompatibleInTime.push_back( *iteh );
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerGlobal  hit OK. "; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerGlobal  hit NOT OK. ";	  
#endif
    }
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) 
      G4cout << " hit time= " << hitTime 
	     << " <? lowestTime= " << lowestTime 
	     << " >=? highestTime= " << highestTime 
	     << " is DeadTimeDetUnit= " << bDeadDU 
	     << "  N hitscomp= " << theHitsCompatibleInTime.size()
	     << " HIT evtID= " << (*iteh)->GetEventID() 
	       << " DU= " << (*iteh)->GetDetUnitID() 
	     << " E= " << (*iteh)->GetEnergy() << G4endl;
#endif
    
  }
}


//----------------------------------------------------------------------
void GmHitList::BuildHitsCompatibleInTimeTriggerIndependent( G4double currentTime )
{
  //----- Build list hits whose time is in the current measuring interval and are not in det units with a recent hit (more recent than theDeadTime )

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent  currentTime= " << currentTime << " theCurrentIntervalStartTime= " << theCurrentIntervalStartTime << G4endl;
#endif

  //----- Build list of hits trigger IDs 
  std::vector<GmHit*> hitsInEvent = GmHitsEventMgr::GetInstance()->GetHitsInEvent(theRunManager->GetCurrentEvent()->GetEventID());
  std::vector<GmHit*>::const_iterator iteh;
  std::set<unsigned long long> hitTrigIDInEvent;
  for( iteh = hitsInEvent.begin(); iteh != hitsInEvent.end(); iteh++ ) {
    if( (*iteh)->GetSDType() == theSDType ) {
      unsigned long long hitTrigID = (*iteh)->GetDetUnitID() / G4int(pow(double(theNShift),theNTrigAncestors));
      hitTrigIDInEvent.insert( hitTrigID );
      if( theHitTrigIDs.find( hitTrigID ) == theHitTrigIDs.end() ) {
	theHitTrigIDs.insert( hitTrigID );
	thePreviousEventIntervalTimes[hitTrigID] = -theMeasuringTime;
      }
      //      G4cout << " HITTRIGID " << hitTrigID << " DetUnitID " <<  (*iteh)->GetDetUnitID() << G4endl;
    }
  }
  
  //----- Select which time interval is being used
  std::map<unsigned long long, G4double>::const_iterator itetm;
  std::set<unsigned long long>::const_iterator iteti;
  for( iteti = theHitTrigIDs.begin(); iteti != theHitTrigIDs.end(); iteti++ ){
    unsigned long long trigID = *iteti;

#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) 
      G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent  currentTime= " << currentTime
	     << "  TrigID " << trigID  
	     << "  theCurrentIntervalStartTime= " << theCurrentIntervalStartTimes[trigID]
	     << " thePreviousEventIntervalTimes= " << thePreviousEventIntervalTimes[trigID] << G4endl;
#endif
    
    //only change the variables of the trigID of hits in this event

    if( currentTime > thePreviousEventIntervalTimes[trigID] + theMeasuringTime ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent trigID " << trigID 
				     << "  change theCurrentIntervalStartTime= " <<  thePreviousEventIntervalTimes[trigID] + theMeasuringTime 
				     << " old " << theCurrentIntervalStartTimes[trigID] 
				     << G4endl;
#endif
      theCurrentIntervalStartTimes[trigID] = thePreviousEventIntervalTimes[trigID] + theMeasuringTime;
      if( hitTrigIDInEvent.find(trigID) != hitTrigIDInEvent.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent trigID " << trigID 
				     << "  change thePreviousEventIntervalTimes " << currentTime-1.E-6*theMeasuringTime
				     << " old " << thePreviousEventIntervalTimes[trigID]
				     << G4endl;
#endif
	thePreviousEventIntervalTimes[trigID] = currentTime-1.E-6*theMeasuringTime; // to avoid precision problems
      }
    }
    
    G4double lowestTime = theCurrentIntervalStartTimes[trigID] - theMeasuringTime;
    G4double highestTime = theCurrentIntervalStartTimes[trigID];
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent  looking between times " << lowestTime << " - " << highestTime << G4endl;
#endif
    //-  G4cout << " ABuildHitsCompatibleInTime " << size() << G4endl;
    for( iteh = begin(); iteh != end(); iteh++ ){
      unsigned long long hitTrigID = (*iteh)->GetDetUnitID() / G4int(pow(double(theNShift),int(theNTrigAncestors)));
      if( hitTrigID != trigID ) continue;

      G4bool bOK = true;
      G4double hitTime = (*iteh)->GetTime();
      G4bool bDeadDU = theDeadTimeDetUnitList->FindDetUnit(*iteh,theDeadTime); 
      if( bDeadDU ) {
	bOK = false;
	(*iteh)->SetDeadTimeFound(true);
      }
      if( hitTime < lowestTime || hitTime >= highestTime ) {
	bOK = false;
      }
      if( bOK == true) {
	theHitsCompatibleInTime.push_back( *iteh );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent  hit OK. "; 
#endif
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(infoVerb) ) G4cout << "GmHitList::BuildHitsCompatibleInTimeTriggerIndependent  hit NOT OK. ";	  
#endif
      }
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) 
	G4cout << " trigID " << trigID
	       << " hit time= " << hitTime 
	       << " <? lowestTime= " << lowestTime 
	       << " >=? highestTime= " << highestTime 
	       << " is DeadTimeDetUnit= " << bDeadDU 
	       << "  N hitscomp= " << theHitsCompatibleInTime.size()
	       << " HIT evtID= " << (*iteh)->GetEventID() 
	       << " DU= " << (*iteh)->GetDetUnitID() 
	       << " E= " << (*iteh)->GetEnergy() << G4endl;
#endif
      
    }
  }
}


//----------------------------------------------------------------------
void GmHitList::DeleteHit( iterator hitite )
{
  //----- delete it also from list of CompatibleHitsInTime
  hitVector::iterator iteh;
  for( iteh = theHitsCompatibleInTime.begin(); iteh != theHitsCompatibleInTime.end(); iteh++ ){
    if( (*iteh) == *hitite ){
      //--	G4cout << " deleting hit c " << *(*iteh) << G4endl;
      theHitsCompatibleInTime.erase( iteh );
      break;
    }
  }

  delete *hitite;
  erase( hitite );

}

//----------------------------------------------------------------------
G4double GmHitList::GetTriggerTime( G4double hitTime, unsigned long long detUnitID )
{
  if( theMeasuringType == bMTBackwards ) {
    return hitTime;
  } else if( theMeasuringType == bMTInterval ) {
    return theCurrentIntervalStartTime - theMeasuringTime;
  } else if( theMeasuringType == bMTTriggerGlobal ) {
    if( hitTime > thePreviousEventIntervalTime + theMeasuringTime ) {
      G4cout << " GetTriggerTime " << hitTime << G4endl;
      return hitTime;
    } else {
      G4cout << " GetTriggerTime " << theCurrentIntervalStartTime << " " <<  theMeasuringTime << G4endl;
      return theCurrentIntervalStartTime - theMeasuringTime;
    }
  } else if( theMeasuringType == bMTTriggerIndependent ) {
    //----- Build list of hits trigger IDs 
    std::vector<GmHit*> hitsInEvent = GmHitsEventMgr::GetInstance()->GetHitsInEvent(theRunManager->GetCurrentEvent()->GetEventID());
    std::vector<GmHit*>::const_iterator iteh;
    std::set<unsigned long long> hitTrigIDInEvent;
    for( iteh = hitsInEvent.begin(); iteh != hitsInEvent.end(); iteh++ ) {
      if( (*iteh)->GetSDType() == theSDType ) {
	unsigned long long hitTrigID = (*iteh)->GetDetUnitID() / G4int(pow(float(theNShift),int(theNTrigAncestors)));
	hitTrigIDInEvent.insert( hitTrigID );
	if( theHitTrigIDs.find( hitTrigID ) == theHitTrigIDs.end() ) {
	  theHitTrigIDs.insert( hitTrigID );
	  thePreviousEventIntervalTimes[hitTrigID] = -theMeasuringTime;
	}
	//      G4cout << " HITTRIGID " << hitTrigID << " DetUnitID " <<  (*iteh)->GetDetUnitID() << G4endl;
      }
    }
    unsigned long long hitTrigID = detUnitID / G4int(pow(float(theNShift),int(theNTrigAncestors)));

    return theCurrentIntervalStartTimes[hitTrigID] - theMeasuringTime;
  }
  
  return 0; // to avoid warnings at compilation
    
}
