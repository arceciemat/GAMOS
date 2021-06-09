#include "GmUserEventActionList.hh"
#include "GmUserEventAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"


//-----------------------------------------------------------------------
GmUserEventActionList::GmUserEventActionList() 
{
}

//-----------------------------------------------------------------------
GmUserEventActionList::~GmUserEventActionList() 
{
}

//-----------------------------------------------------------------------
void GmUserEventActionList::BeginOfEventAction(const G4Event* anEvent) 
{

  vUEventA::iterator aite;
  for(aite = theEventActions.begin(); aite != theEventActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Begin of EVENT action executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->BeginOfEventAction(anEvent);
  }
}

//-----------------------------------------------------------------------
void GmUserEventActionList::EndOfEventAction(const G4Event* anEvent) 
{
  vUEventA::iterator aite;
  for(aite = theEventActions.begin(); aite != theEventActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " End of EVENT action executing " << *aite << " " << (*aite)->GetName() << " ENABLED " << (*aite)->IsEnabled() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->EndOfEventAction(anEvent);
  }
}


//-----------------------------------------------------------------------
void GmUserEventActionList::AddAction( GmUserEventAction* ua )
{
  theEventActions.push_back( ua );
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserEventActionList::AddAction " << ua << " N " << theEventActions.size() << G4endl;
#endif
  //--- first user action, register it to G4RunManager
  if( theEventActions.size() == 1) G4RunManager::GetRunManager()->SetUserAction( this );
  ua->SetEventManager( fpEventManager );
}


//-----------------------------------------------------------------------
void GmUserEventActionList::RemoveAction(  GmUserEventAction* ua )
{
  vUEventA::iterator ite = find(theEventActions.begin(), theEventActions.end(), ua );
  if(ite != theEventActions.end() ) { 
    theEventActions.erase( ite );
    //--- No user actions in list, unregister it to G4RunManager
    if( theEventActions.size() == 0) G4RunManager::GetRunManager()->SetUserAction( (G4UserEventAction*)0 );
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(warningVerb) ) G4cerr << " !!WARNING UserEventActionaction is not in List " << ua->GetName() << G4endl;
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserEventActionList::RemoveAction " << ua << " N " << theEventActions.size() << G4endl;
#endif
}


//-----------------------------------------------------------------------
GmUserEventAction* GmUserEventActionList::FindAction( const G4String& name) const
{
  vUEventA::const_iterator ite;
  for( ite = theEventActions.begin(); ite != theEventActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      return *ite;
    }
  }

  return 0;

}
 

//------------------------------------------------------------------------
G4int GmUserEventActionList::CountActions( const G4String& name ) const
{
  G4int nAct = 0;
  vUEventA::const_iterator ite;
  for( ite = theEventActions.begin(); ite != theEventActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      nAct++;
    }
  }
  
  return nAct;

}

//-----------------------------------------------------------------------
G4int GmUserEventActionList::DisableAction( const G4String& name)
{
  G4int nActions = 0;
  vUEventA::iterator ite;
  for( ite = theEventActions.begin(); ite != theEventActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Disable();
      nActions++;
    }
  }

  return nActions;

}
 

//-----------------------------------------------------------------------
G4int GmUserEventActionList::EnableAction( const G4String& name)
{
  G4int nActions = 0;
  vUEventA::iterator ite;
  for( ite = theEventActions.begin(); ite != theEventActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Enable();
      nActions++;
    }
  }

  return nActions;

}
 

//-----------------------------------------------------------------------
void GmUserEventActionList::DumpActions( std::ostream& out, G4int enabled)
{
  G4String onoff[2] = {"OFF","ON"};
  vUEventA::iterator ite;
  for( ite = theEventActions.begin(); ite != theEventActions.end(); ite++ ) { 
    if( enabled == 0 
	|| (enabled == 1 && (*ite)->IsEnabled() )
	|| (enabled == -1 && !(*ite)->IsEnabled() ) ) {
      out << " " << (*ite)->GetName() << " " << onoff[ int((*ite)->IsEnabled()) ] << G4endl;
    }
 }

}
