#include "GmUserTrackingActionList.hh"
#include "GmUserTrackingAction.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "G4Track.hh"
#include "G4RunManager.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

//-----------------------------------------------------------------------
GmUserTrackingActionList::GmUserTrackingActionList() 
{
}

//-----------------------------------------------------------------------
GmUserTrackingActionList::~GmUserTrackingActionList() 
{
}

//-----------------------------------------------------------------------
void GmUserTrackingActionList::PreUserTrackingAction(const G4Track* aTrack) 
{
  vUTrackingA::iterator aite;
  std::vector<GmVFilter*>::const_iterator ite;
  for(aite = theTrackingActions.begin(); aite != theTrackingActions.end(); aite++ ){
    G4bool bAccept = TRUE;
    if( (*aite)->ApplyFiltersToPreTracking() ) {
      for( ite = (*aite)->theFilters.begin(); ite != (*aite)->theFilters.end(); ite++ ){
	if( !(*ite)->AcceptTrack(aTrack) ) {
	  bAccept = FALSE;
	  if( !(*aite)->CheckAllFiltersAtPreTracking() ) break;
	}
      } 
    }
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Pre Tracking action executing " << *aite << " " << (*aite)->GetName() << " bAccept " << bAccept << G4endl;
#endif
    if( bAccept ) (*aite)->PreUserTrackingAction(aTrack);
  }
}

//-----------------------------------------------------------------------
void GmUserTrackingActionList::PostUserTrackingAction(const G4Track* aTrack) 
{
  vUTrackingA::iterator aite;
  std::vector<GmVFilter*>::const_iterator ite;
  for(aite = theTrackingActions.begin(); aite != theTrackingActions.end(); aite++ ){
    G4bool bAccept = TRUE;
    if( (*aite)->ApplyFiltersToPostTracking() ) {
      for( ite = (*aite)->theFilters.begin(); ite != (*aite)->theFilters.end(); ite++ ){
	if( !(*ite)->AcceptTrack(aTrack) ) {
	  bAccept = FALSE;
	  if( !(*aite)->CheckAllFiltersAtPostTracking() ) break;
	}
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Post Tracking action executing " << *aite << " " << (*aite)->GetName() << " bAccept " << bAccept << G4endl;
#endif
    if( bAccept ) (*aite)->PostUserTrackingAction(aTrack);
  }
}


//-----------------------------------------------------------------------
void GmUserTrackingActionList::AddAction( GmUserTrackingAction* ua )
{
    theTrackingActions.push_back( ua );
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " GmUserTrackingActionList::AddAction " << ua << " N " << theTrackingActions.size() << G4endl;
#endif
    //--- first user action, register it to G4RunManager
    if( theTrackingActions.size() == 1) G4RunManager::GetRunManager()->SetUserAction( this );
    ua->SetTrackingManager( fpTrackingManager );
    // }
}


//-----------------------------------------------------------------------
void GmUserTrackingActionList::RemoveAction(  GmUserTrackingAction* ua )
{
  vUTrackingA::iterator ite = find(theTrackingActions.begin(), theTrackingActions.end(), ua );
  if(ite != theTrackingActions.end() ) { 
    theTrackingActions.erase( ite );
    //--- No user actions in list, unregister it to G4RunManager
    if( theTrackingActions.size() == 0) G4RunManager::GetRunManager()->SetUserAction( (G4UserTrackingAction*)0 );
  } else {
    G4cerr << " !!WARNING UserTrackingActionaction is not in List " << ua->GetName() << G4endl;
  }
}


//-----------------------------------------------------------------------
GmUserTrackingAction* GmUserTrackingActionList::FindAction( const G4String& name) const
{
  vUTrackingA::const_iterator ite;
  for( ite = theTrackingActions.begin(); ite != theTrackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      return *ite;
    }
  }
  
  return 0;

}
 
//------------------------------------------------------------------------
G4int GmUserTrackingActionList::CountActions( const G4String& name ) const
{
  G4int nAct = 0;
  vUTrackingA::const_iterator ite;
  for( ite = theTrackingActions.begin(); ite != theTrackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      nAct++;
    }
  }
  
  return nAct;

}


//-----------------------------------------------------------------------
G4int GmUserTrackingActionList::DisableAction( const G4String& name)
{

  G4int nActions = 0;
  vUTrackingA::iterator ite;
  for( ite = theTrackingActions.begin(); ite != theTrackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Disable();
      nActions++;
    }
  }

  return nActions;

}

//-----------------------------------------------------------------------
G4int GmUserTrackingActionList::EnableAction( const G4String& name)
{
  G4int nActions = 0;
  vUTrackingA::iterator ite;
  for( ite = theTrackingActions.begin(); ite != theTrackingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Enable();
      nActions++;
    }
  }

  return nActions;

}
 
//-----------------------------------------------------------------------
void GmUserTrackingActionList::DumpActions( std::ostream& out, G4int enabled)
{
  G4String onoff[2] = {"OFF","ON"};
  vUTrackingA::iterator ite;
  for( ite = theTrackingActions.begin(); ite != theTrackingActions.end(); ite++ ) { 
    if( enabled == 0 
	|| (enabled == 1 && (*ite)->IsEnabled() )
	|| (enabled == -1 && !(*ite)->IsEnabled() ) ) {
      out << " " << (*ite)->GetName() << " " << onoff[ int((*ite)->IsEnabled()) ] << G4endl;
    }
 }

}

 
