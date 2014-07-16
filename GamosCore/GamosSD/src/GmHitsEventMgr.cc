#include "GmHitsEventMgr.hh"
#include "GmVSDList.hh"
#include "GmHitList.hh"
#include "GmVDigitizer.hh"
#include "GmVRecHitBuilderFromHits.hh"
#include "GmVRecHitBuilderFromDigits.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromFirstTrack.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Event.hh"

#include "Reflex/PluginService.h"

//------------------------------------------------------------------------
GmHitsEventMgr* GmHitsEventMgr::theInstance = 0;

//------------------------------------------------------------------------
GmHitsEventMgr* GmHitsEventMgr::GetInstance()
{
  if( theInstance == 0 ){
    theInstance = new GmHitsEventMgr;
  }

  return theInstance;

}

//------------------------------------------------------------------------
GmHitsEventMgr::GmHitsEventMgr()
{
  theEventTime = -1.; // if it is not reset, time is not taken into account (only hits of current event are considered)
  
  GmUserAction* chk = Reflex::PluginService::Create<GmUserAction*>("GmCheckOriginalGamma");
  //  G4cout << this << " theEventTimeExtractor " << theEventTimeExtractor << G4endl;
  theEventTimeExtractor = new GmEventTimeExtractorFromFirstTrack();

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4String tt = parmgr->GetStringValue("SD:Hits:TimeType","min");
  if( tt == "min" || tt == "MIN" ) {
    theHitsTimeType = 1;
  } else if( tt == "max" || tt == "MAX" ) {
    theHitsTimeType = 2;
  } else {
    G4Exception("GmHitsEventMgr::GmHitsEventMgr",
		"ERROR setting parameter 'SD:Hits:TimeType'",
		FatalException,
		G4String("It can only be min/MIN, max/MAX, it is "+tt).c_str());
  }

}


//------------------------------------------------------------------------
GmHitsEventMgr::~GmHitsEventMgr()
{
}


//------------------------------------------------------------------------
void GmHitsEventMgr::AddDigitizer( G4String& name, G4String& sdtype )
{
  G4String sdtyp = "/"+sdtype+"/";
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout <<"  GmHitsEventMgr::AddDigitizer name= " << name << " type= " << sdtyp << G4endl;
#endif
  //----- check sd type exits
  if( theHitLists.find( sdtyp ) == theHitLists.end() ) {
    G4cerr << "!!! WARNING at GmHitsEventMgr:::AddDigitizer  no sensitive detector of type " << sdtype << " is defined, no hits will be created (and then no hits will be digitized). Please check your input command " << G4endl;
  }
  
  GmVDigitizer* digi = Reflex::PluginService::Create<GmVDigitizer*>(name);
  digi->SetName( name + "_" + sdtype );
  digi->SetParams();
  digi->SetSDType( sdtyp );
  if( !digi ) {
    G4Exception("GmHitsEventMgr::AddRecHitBuilder",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("No recHitBuilder found of type " + name).c_str());
  }
 theDigitizers[sdtyp] = digi;

}

//------------------------------------------------------------------------
void GmHitsEventMgr::AddRecHitBuilder( G4String& name, G4String& sdtype )
{
  G4String sdtyp = "/"+sdtype+"/";
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmHitsEventMgr::AddRecHitBuilder name= " << name << " type= " << sdtyp << G4endl;
#endif
  //----- check sd type exits
  if( theHitLists.find( sdtyp ) == theHitLists.end() ) {
    G4cerr << "!!! WARNING at GmHitsEventMgr::AddRecHitBuilder:  no sensitive detector of type " << sdtype << " is defined, no hits will be created (and then no hits will be reconstructed). Please check your input command " << G4endl;
  }

  GmVRecHitBuilder* rhB = Reflex::PluginService::Create<GmVRecHitBuilder*>(name);
  rhB->SetName( name + "_" + sdtype );
  rhB->SetParams();
  rhB->SetSDType( sdtyp );
  if( !rhB ) {
    G4Exception("GmHitsEventMgr::AddRecHitBuilder",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("No recHitBuilder found of type " + name).c_str());
  }
  theRecHitBuilders[sdtyp] = rhB;

}

//------------------------------------------------------------------------
GmHitList* GmHitsEventMgr::CreateHitList( G4String sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitsEventMgr::CreateHitList of type " << sdtype << G4endl;
#endif
  gamosSDHitMap::const_iterator iteh = theHitLists.find( sdtype );
  if( iteh == theHitLists.end() ) {
    theHitLists[sdtype] = new GmHitList( sdtype );
    return theHitLists[sdtype];
  } else {
    return (*iteh).second;
  }
}

//------------------------------------------------------------------------
void GmHitsEventMgr::AddHit( GmHit* hit, G4String sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout <<" adding hit to " << sdtype << " hitslist size " << theHitLists[sdtype]->size() << G4endl;
#endif
  theHitLists[sdtype]->AddHit( hit );

} 


//------------------------------------------------------------------------
void GmHitsEventMgr::BuildHitsCompatibleInTime()
{
#ifndef GAMOS_NO_VERBOSE
  //-  if( SDVerb(debugVerb) ) G4cout << " GmHitsEventMgr::BuildHitsCompatibleInTime time " << " hits groups " << theHitLists.size() << G4endl;
#endif
  gamosSDHitMap::const_iterator iteh;
  for( iteh = theHitLists.begin(); iteh != theHitLists.end(); iteh++ ){
    GmHitList* hitlist = (*iteh).second;
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << "GmHitsEventMgr::BuildHitsCompatibleInTime for SD " << (*iteh).first << " Nhits= " << hitlist->size() << " event time= " << theEventTime << G4endl;
#endif
    hitlist->BuildHitsCompatibleInTime( theEventTime ); // build them and store them on HitsEventMgr 
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(debugVerb) ) G4cout << " GmHitsEventMgr::BuildHitsCompatibleInTime for SD " << (*iteh).first << " Nhits compatible found= " << hitlist->GetHitsCompatibleInTime()->size() << G4endl;
#endif
  }
}


//------------------------------------------------------------------------
void GmHitsEventMgr::DigitizeAndReconstructHits()
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmHitsEventMgr::DigitizeAndReconstructHits() " << G4endl;
#endif

  //-------- Digitize and reconstruct hits
  //--- Loop to hit lists (one per SD type)
  gamosSDHitMap::const_iterator iteh;
  std::map< G4String, GmVDigitizer* >::const_iterator itedig; 
  std::map< G4String, GmVRecHitBuilder* >::const_iterator iterh; 
 
  for( iteh = theHitLists.begin(); iteh != theHitLists.end(); iteh++ ){
    GmHitList* hitlist = (*iteh).second;
    //----- If digitizer found, digitize and reconstruct digits
    itedig = theDigitizers.find( (*iteh).first );
    if( itedig != theDigitizers.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << " Digitizing hits " << ((*itedig).second) << G4endl;
#endif
      theDigits[ (*iteh).first] = ((*itedig).second)->DigitizeHits( hitlist->GetHitsCompatibleInTime() );
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << " Digitized hits " << theDigits[ (*iteh).first].size() << G4endl;
#endif
      
      iterh = theRecHitBuilders.find( (*iteh).first );
      if( iterh != theRecHitBuilders.end() ) {
	GmVRecHitBuilderFromDigits* rhitBuilder = dynamic_cast<GmVRecHitBuilderFromDigits*>((*iterh).second);
	theRecHits[ (*iteh).first] = rhitBuilder->ReconstructDigits( &(theDigits[(*iteh).first]) );
	rhitBuilder->SmearRecHitsEnergy();
	rhitBuilder->SmearRecHitsTime();
      }
      
      //----- If no digitizer found, reconstruct hits
    } else {
      iterh = theRecHitBuilders.find( (*iteh).first );
#ifndef GAMOS_NO_VERBOSE
      //-      if( SDVerb(debugVerb) ) G4cout << " theRecHitBuilders.size() " << theRecHitBuilders.size() << G4endl;
#endif
      if( iterh != theRecHitBuilders.end() ) {
	GmVRecHitBuilderFromHits* rhitBuilder = dynamic_cast<GmVRecHitBuilderFromHits*>((*iterh).second);
	const std::vector<GmHit*>* hitsCompatible = hitlist->GetHitsCompatibleInTime();
	if( hitsCompatible->size() != 0 ) {
	  theRecHits[ (*iteh).first] = rhitBuilder->ReconstructHits( hitsCompatible );
	}
	rhitBuilder->CheckRecHitsMinEnergy();
	rhitBuilder->SmearRecHitsEnergy();
	rhitBuilder->SmearRecHitsTime();
	rhitBuilder->CheckEnergyEfficiency();
      }
    }

  } 
}


//------------------------------------------------------------------------
void GmHitsEventMgr::AddHitToDeadTimeDetUnitList( const G4String& sdtype )
{
  theHitLists[sdtype]->AddHitToDeadTimeDetUnitList();
}


//------------------------------------------------------------------------
GmHitList* GmHitsEventMgr::GetHitList( const G4String& sdType, G4bool bMustExist )
{
  G4String sdTyp = "/"+sdType+"/";
  gamosSDHitMap::const_iterator ite = theHitLists.find(sdTyp); 
  if( ite == theHitLists.end() ){
    if( bMustExist ) {
      for( ite = theHitLists.begin(); ite != theHitLists.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout<< " HitList " << (*ite).first << G4endl;
#endif
      }
      G4Exception("GmHitsEventMgr::GetHits",
		  "Error",
		  FatalException,
		  ("No hit list for sensitive detectory type " + sdType).c_str() );
    }
  }
  return (*ite).second;
}


//------------------------------------------------------------------------
std::vector<GmHit*> GmHitsEventMgr::GetHitsInEvent( G4int evtID )
{
  std::vector<GmHit*> hits;
  gamosSDHitMap::const_iterator item;
  GmHitList::reverse_iterator itel;
  for( item = theHitLists.begin(); item != theHitLists.end(); item++ ){
    GmHitList* hitlist = (*item).second;
    for( itel = hitlist->rbegin(); itel != hitlist->rend(); itel++ ){
      if( (*itel)->GetEventID() == evtID ) {
        hits.push_back( *itel );
      } else if( (*itel)->GetEventID() < evtID ) {
        break; // it event ID is smaller, all the other will also be
      }
    }
  }

  return hits;
}

/*//------------------------------------------------------------------------
std::vector<GmHit*> GmHitsEventMgr::GetAllHits()
{
  std::vector<GmHit*> hits;
  gamosSDHitMap::const_iterator item;
  GmHitList::reverse_iterator itel;
  for( item = theHitLists.begin(); item != theHitLists.end(); item++ ){
    GmHitList* hitlist = (*item).second;
    for( itel = hitlist->rbegin(); itel != hitlist->rend(); itel++ ){
      hits.push_back( *itel );
    }
  }

  return hits;
}
*/

//------------------------------------------------------------------------
std::vector<GmDigit*> GmHitsEventMgr::GetDigits( const G4String& sdType, G4bool bMustExist )
{
  G4String sdTyp = "/"+sdType+"/";
  gamosSDDigitMap::const_iterator ite = theDigits.find(sdTyp); 
  if( ite == theDigits.end() ){
    //--- Check if digitizer exists
    if( theDigitizers.find(sdType) == theDigitizers.end() ) {
      if( bMustExist ) {
	for( ite = theDigits.begin(); ite != theDigits.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	  if( SDVerb(debugVerb) ) G4cout << " Digit List " << (*ite).first << G4endl;
#endif
	}
	G4Exception("GmHitsEventMgr::GetDigits",
		    "Error",
		    FatalException,
		    ("No digitizer for sensitive detectory type " + sdType).c_str() );
      }
    } else {
      std::vector<GmDigit*> digits;
      return digits;
    }
  } else {
    return (*ite).second; 
  }

  std::vector<GmDigit*> digits;
  return digits;
}

//------------------------------------------------------------------------
std::vector<GmRecHit*> GmHitsEventMgr::GetRecHits( const G4String& sdType, G4bool bMustExist )
{
  G4String sdTyp = "/"+sdType+"/";
  gamosSDRecHitMap::const_iterator ite = theRecHits.find(sdTyp); 
  if( ite == theRecHits.end() ){
    if( theRecHitBuilders.find(sdType) == theRecHitBuilders.end() ) {
      if( bMustExist ) {
	for( ite = theRecHits.begin(); ite != theRecHits.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	  if( SDVerb(debugVerb) ) G4cout << " RecHit List " << (*ite).first << G4endl;
#endif
	}
	G4Exception("GmHitsEventMgr::GetRecHits",
		    "Error",
		    FatalException,
		    ("No recHit builder for sensitive detectory type " + sdType).c_str() );
      }
    } else {
      std::vector<GmRecHit*> rechits;
      return rechits;
    }
  } else {
    return (*ite).second; 
  }

  std::vector<GmRecHit*> rechits;
  return rechits;

}


//------------------------------------------------------------------------
std::vector<GmRecHit*> GmHitsEventMgr::GetAllRecHits()
{
  std::vector<GmRecHit*> rechits;
  gamosSDRecHitMap::const_iterator ite;
  std::vector<GmRecHit*>::iterator itev;
  for(ite = theRecHits.begin(); ite != theRecHits.end(); ite++ ){
    std::vector<GmRecHit*> rechitssd = (*ite).second;
    for( itev = rechitssd.begin(); itev != rechitssd.end(); itev++ ){
      rechits.push_back(*itev);
    }
  }

  return rechits;
}


//------------------------------------------------------------------------
void GmHitsEventMgr::CleanHits()
{
  //----- Clean hits of each hitlist
  gamosSDHitMap::iterator itehm;
  for( itehm = theHitLists.begin(); itehm != theHitLists.end(); itehm++ ){
    ((*itehm).second)->CleanHits( theEventTime );
  }

}


//------------------------------------------------------------------------
void GmHitsEventMgr::CleanDigitsAndRecHits()
{
  std::map< G4String, GmVDigitizer* >::const_iterator itedig; 
  for( itedig = theDigitizers.begin(); itedig != theDigitizers.end(); itedig++ ){
    ((*itedig).second)->CleanDigits();
  }
  theDigits.clear();
  
  std::map< G4String, GmVRecHitBuilder* >::const_iterator iterh; 
  for( iterh = theRecHitBuilders.begin(); iterh != theRecHitBuilders.end(); iterh++ ){
    ((*iterh).second)->CleanRecHits();
  }

  theRecHits.clear();
}


//------------------------------------------------------------------------
void GmHitsEventMgr::CleanDeadTimeDetUnitList()
{
  gamosSDHitMap::const_iterator itehm;
  for( itehm = theHitLists.begin(); itehm != theHitLists.end(); itehm++ ){
  //----- Clean old hits
    ((*itehm).second)->CleanDeadTimeDetUnitList( theEventTime );
  }
  
}


//------------------------------------------------------------------------
void GmHitsEventMgr::DeleteHits( GmRecHit* rhit )
{
  std::vector<GmHit*>::iterator ite;
  std::vector<GmHit*> hits = rhit->GetHits();

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmHitsEventMgr::DeleteHits in recHit Nhits= " << hits.size() << G4endl;
#endif

  for( ite = hits.begin(); ite != hits.end(); ite++ ){
    DeleteHit( *ite );
  }

  //delete rechit
  gamosSDRecHitMap::iterator iterh;
  std::vector<gamosSDRecHitMap::iterator> iterhToDelete;
  std::vector<GmRecHit*>::iterator itev;
  for( iterh = theRecHits.begin(); iterh != theRecHits.end(); iterh++ ){
    std::vector<GmRecHit*>* rechitsSD = &((*iterh).second);
    for(itev = rechitsSD->begin(); itev != rechitsSD->end(); itev++ ){
      if( *itev == rhit ) {
	rechitsSD->erase( itev );
	break;
      }
    }
    if( rechitsSD->size() == 0 ) iterhToDelete.push_back(iterh);
  }

  for( G4int ii = iterhToDelete.size()-1; ii >= 0; ii--){
    theRecHits.erase( iterhToDelete[ii] );
  }
  
  std::map< G4String, GmVRecHitBuilder* >::iterator iterhb = theRecHitBuilders.find( rhit->GetSDType() );
  (*iterhb).second->DeleteHit( rhit );
}


//------------------------------------------------------------------------
void GmHitsEventMgr::DeleteHit( GmHit* hit )
{
  gamosSDHitMap::const_iterator itehmp;
  for( itehmp = theHitLists.begin(); itehmp != theHitLists.end(); itehmp++ ){
    GmHitList* hitlist = (*itehmp).second;
    GmHitList::iterator iteh;
    //--- Loop to hits
    for( iteh = hitlist->begin(); iteh != hitlist->end(); iteh++ ){
      if( (*iteh) == hit ){
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(infoVerb) ) G4cout << " GmHitsEventMgr::DeleteHit " << (*iteh)->GetEnergy() << " " << (*iteh)->GetTime() << G4endl;
#endif
	hitlist->DeleteHit( iteh );
	break;
      }
    }
  }

}


//------------------------------------------------------------------------
void GmHitsEventMgr::BuildEventTime( const G4Event* evt )
{
  theEventTime = theEventTimeExtractor->GetEventTime(evt);
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmHitsEventMgr::BuildEventTime = " << theEventTime << G4endl;
#endif

}

