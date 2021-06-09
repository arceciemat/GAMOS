#include "GmUserRunActionList.hh"
#include "GmUserRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include <iostream>

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"


//-----------------------------------------------------------------------
GmUserRunActionList::GmUserRunActionList() 
{
}

//-----------------------------------------------------------------------
GmUserRunActionList::~GmUserRunActionList() 
{
}

//-----------------------------------------------------------------------
void GmUserRunActionList::BeginOfRunAction(const G4Run* aRun) 
{
  vURunA::iterator aite;
  for(aite = theRunActions.begin(); aite != theRunActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " Begin of RUN action executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->BeginOfRunAction(aRun);
  }
}

//-----------------------------------------------------------------------
void GmUserRunActionList::EndOfRunAction(const G4Run* aRun) 
{
  vURunA::iterator aite;
  for(aite = theRunActions.begin(); aite != theRunActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " End of RUN action executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) (*aite)->EndOfRunAction(aRun);
  }

}

//-----------------------------------------------------------------------
G4Run* GmUserRunActionList::GenerateRun()
{
  G4Run* run = 0;

  vURunA::iterator aite;
  for(aite = theRunActions.begin(); aite != theRunActions.end(); aite++ ){
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " GenerateRun executing " << *aite << " " << (*aite)->GetName() << G4endl;
#endif
    if( (*aite)->IsEnabled() ) {
      G4Run* newRun = (*aite)->GenerateRun();
      if( newRun != 0 ) {
	if( run != 0 ){
	  G4Exception("GmUserRunActionList::GenerateRun()","Two Run Actions create a G4Run",JustWarning,(" Only G4Run of this action will be taken "+(*aite)->GetName()).c_str());
	}
	run = newRun;
      } 
    }
  }

  return run;
}


//-----------------------------------------------------------------------
void GmUserRunActionList::AddAction( GmUserRunAction* ua )
{
  theRunActions.push_back( ua );
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserRunActionList::AddAction " << ua << " N " << theRunActions.size() << G4endl;
#endif
  //--- first user action, register it to G4RunManager
  if( theRunActions.size() == 1) G4RunManager::GetRunManager()->SetUserAction( this );
}


//-----------------------------------------------------------------------
void GmUserRunActionList::RemoveAction(  GmUserRunAction* ua )
{
  vURunA::iterator ite = find(theRunActions.begin(), theRunActions.end(), ua );
  if(ite != theRunActions.end() ) { 
    theRunActions.erase( ite );
    //--- No user actions in list, unregister it to G4RunManager
    if( theRunActions.size() == 0) G4RunManager::GetRunManager()->SetUserAction( (G4UserRunAction*)0 );
  } else {
    G4cerr << " !!WARNING UserRunActionaction is not in List " << ua->GetName() << G4endl;
  }
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserRunActionList::RemoveAction " << ua << " N " << theRunActions.size() << G4endl;
#endif

}


//-----------------------------------------------------------------------
GmUserRunAction* GmUserRunActionList::FindAction( const G4String& name) const
{
  vURunA::const_iterator ite;
  for( ite = theRunActions.begin(); ite != theRunActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      return *ite;
    }
  }

  return 0;

}
 

//------------------------------------------------------------------------
G4int GmUserRunActionList::CountActions( const G4String& name ) const
{
  G4int nAct = 0;
  vURunA::const_iterator ite;
  for( ite = theRunActions.begin(); ite != theRunActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      nAct++;
    }
  }
  
  return nAct;

}

//-----------------------------------------------------------------------
G4int GmUserRunActionList::DisableAction( const G4String& name)
{
  G4int nActions = 0;
  vURunA::iterator ite;
  for( ite = theRunActions.begin(); ite != theRunActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Disable();
      nActions++;
    }
  }

  return nActions;

}
//-----------------------------------------------------------------------
G4int GmUserRunActionList::EnableAction( const G4String& name)
{
  G4int nActions = 0;
  vURunA::iterator ite;
  for( ite = theRunActions.begin(); ite != theRunActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Enable();
      nActions++;
    }
  }

  return nActions;

}
 
//-----------------------------------------------------------------------
void GmUserRunActionList::DumpActions( std::ostream& out, G4int enabled)
{
  G4String onoff[2] = {"OFF","ON"};
  vURunA::iterator ite;
  for( ite = theRunActions.begin(); ite != theRunActions.end(); ite++ ) { 
    if( enabled == 0 
	|| (enabled == 1 && (*ite)->IsEnabled() )
	|| (enabled == -1 && !(*ite)->IsEnabled() ) ) {
      out << " " << (*ite)->GetName() << " " << onoff[ int((*ite)->IsEnabled()) ] << G4endl;
    }
 }

}
