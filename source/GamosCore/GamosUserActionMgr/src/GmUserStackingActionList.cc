#include "GmUserStackingActionList.hh"
#include "GmUserStackingAction.hh"

#include "G4Track.hh"
#include "G4RunManager.hh"
#include <iostream>
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

//-----------------------------------------------------------------------
GmUserStackingActionList::GmUserStackingActionList() 
{
}

//-----------------------------------------------------------------------
GmUserStackingActionList::~GmUserStackingActionList() 
{
}

//-----------------------------------------------------------------------
void GmUserStackingActionList::PrepareNewEvent()
{
  vUStackingA::iterator aite;
  for(aite = theStackingActions.begin(); aite != theStackingActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Stacking PrepareNewEvent action executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->PrepareNewEvent();
  }
}

//-----------------------------------------------------------------------
G4ClassificationOfNewTrack GmUserStackingActionList::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ClassificationOfNewTrack newClassification = fUrgent;
  std::vector<GmVFilter*>::const_iterator ite;
  vUStackingA::iterator aite;
  for(aite = theStackingActions.begin(); aite != theStackingActions.end();
 aite++ ){
    G4bool bAccept = TRUE;
    if( (*aite)->ApplyFiltersToStacking() ) {
      for( ite = (*aite)->theFilters.begin(); ite != (*aite)->theFilters.end(); ite++ ){
	//	G4cout << " FILTER " << (*ite)->GetName() << G4endl; //GDEB
	if( !(*ite)->AcceptStackedTrack(aTrack) ) {
	  bAccept = FALSE;
	  if( !(*aite)->CheckAllFiltersAtStacking() ) break;
	}
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Stacking ClassifyNewTrack action executing " << *aite << " " << (*aite)->GetName() << " bAccept " << bAccept << G4endl;
#endif
    if( bAccept ) newClassification = (*aite)->ClassifyNewTrack( aTrack);
  }

  return newClassification;
}


//-----------------------------------------------------------------------
void GmUserStackingActionList::NewStage()
{
  vUStackingA::iterator aite;
  for(aite = theStackingActions.begin(); aite != theStackingActions.end();
      aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Stacking NewStage action executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->NewStage();
  }
}


//-----------------------------------------------------------------------
void GmUserStackingActionList::AddAction( GmUserStackingAction* ua )
{
  theStackingActions.push_back( ua );
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserStackingActionList::AddAction " << " N " << theStackingActions.size() << G4endl;
#endif
  //--- first user action, register it to G4StackingManager
  if( theStackingActions.size() == 1) G4RunManager::GetRunManager()->SetUserAction( this );
  ua->SetStackManager( stackManager );
  if( theStackingActions.size() >1) {
    G4Exception("GmUserStackingActionList::AddAction ", 
		" more than 1 action registered",
		JustWarning,
		"The last action will be taken (run /gamos/verbosity GmUAVerbosity debug to get details");
  }
}


//-----------------------------------------------------------------------
void GmUserStackingActionList::RemoveAction(  GmUserStackingAction* ua )
{
  vUStackingA::iterator ite = find(theStackingActions.begin(), theStackingActions.end(), ua );
  if(ite != theStackingActions.end() ) { 
    theStackingActions.erase( ite );
    //--- No user actions in list, unregister it to G4StackingManager
    if( theStackingActions.size() == 0) G4RunManager::GetRunManager()->SetUserAction( (G4UserStackingAction*)0 );
  } else {
    G4cerr << " !!WARNING UserStackingActionaction is not in List " << ua->GetName() << G4endl;
  }
}


//-----------------------------------------------------------------------
GmUserStackingAction* GmUserStackingActionList::FindAction( const G4String& name) const
{
  vUStackingA::const_iterator ite;
  for( ite = theStackingActions.begin(); ite != theStackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      return *ite;
    }
  }

  return 0;

}
 

//------------------------------------------------------------------------
G4int GmUserStackingActionList::CountActions( const G4String& name ) const
{
  G4int nAct = 0;
  vUStackingA::const_iterator ite;
  for( ite = theStackingActions.begin(); ite != theStackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      nAct++;
    }
  }
  
  return nAct;

}


//-----------------------------------------------------------------------
G4int GmUserStackingActionList::DisableAction( const G4String& name)
{
  G4int nActions = 0;
  vUStackingA::iterator ite;
  for( ite = theStackingActions.begin(); ite != theStackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Disable();
      nActions++;
    }
  }

  return nActions;

}

//-----------------------------------------------------------------------
G4int GmUserStackingActionList::EnableAction( const G4String& name)
{
  G4int nActions = 0;
  vUStackingA::iterator ite;
  for( ite = theStackingActions.begin(); ite != theStackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Enable();
      nActions++;
    }
  }

  return nActions;

}
 
//-----------------------------------------------------------------------
void GmUserStackingActionList::DumpActions( std::ostream& out, G4int enabled)
{
  G4String onoff[2] = {"OFF","ON"};
  vUStackingA::iterator ite;
  for( ite = theStackingActions.begin(); ite != theStackingActions.end(); ite++ ) { 
    if( enabled == 0 
	|| (enabled == 1 && (*ite)->IsEnabled() )
	|| (enabled == -1 && !(*ite)->IsEnabled() ) ) {
      out << " " << (*ite)->GetName() << " " << onoff[ int((*ite)->IsEnabled()) ] << G4endl;
    }
 }

}
