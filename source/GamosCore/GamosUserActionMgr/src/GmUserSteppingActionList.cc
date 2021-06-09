#include "GmUserSteppingActionList.hh"
#include "GmUserSteppingAction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include <iostream>
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GmFutureFilter.hh"
#include "GmStepMgr.hh"

//-----------------------------------------------------------------------
GmUserSteppingActionList::GmUserSteppingActionList() 
{
}

//-----------------------------------------------------------------------
GmUserSteppingActionList::~GmUserSteppingActionList() 
{
}


//-----------------------------------------------------------------------
void GmUserSteppingActionList::UserSteppingAction(const G4Step* aStep) 
{

  vUSteppingA::iterator aite;
  std::vector<GmVFilter*>::const_iterator ite;
  for(aite = theSteppingActions.begin(); aite != theSteppingActions.end(); aite++ ){

#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingActionList::UserSteppingAction checking filters " << (*aite)->GetName() << " applyFilters? " << (*aite)->ApplyFiltersToStepping() << G4endl;
#endif

    G4bool bAccept = TRUE;
    if( (*aite)->ApplyFiltersToStepping() ) { // for future filters ?
      for( ite = (*aite)->theFilters.begin(); ite != (*aite)->theFilters.end(); ite++ ){
	if( !(*ite)->IsFutureFilter() ) {
	  if( !(*ite)->AcceptStep(aStep) ) {
	    bAccept = FALSE;
	    if( !(*aite)->CheckAllFiltersAtStepping() ) break;
	  }
       
#ifndef GAMOS_NO_VERBOSE
	  if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingActionList::UserSteppingAction " << " " << (*aite)->GetName() << " Filter " << (*ite)->GetName() << " AcceptFilter? " << bAccept << G4endl;
#endif
	  
	} else {

	  if( (*aite)->theFilters.size() > 1 ) {
	    for( ite = (*aite)->theFilters.begin(); ite != (*aite)->theFilters.end(); ite++ ){
	      G4cerr << " GmUserSteppingActionList name= " << (*aite)->GetName() << " Filter= " << (*ite)->GetName() << G4endl;
	    }
	    G4Exception("GmUserSteppingActionList",
			"FutureFilter must be the only filter",
			FatalErrorInArgument,
			"");
	  }
	  GmStepMgr* stepMgr = GmStepMgr::GetInstance();
	  GmFutureFilter * futureFilter = (GmFutureFilter*)(*ite);
	  //-	  if( futureFilter->AcceptPastStep(aStep) || futureFilter->IsFilterToChildren() ) stepMgr->SaveStep( aStep );
	  
	  bAccept = futureFilter->AcceptFutureStep(aStep);
#ifndef GAMOS_NO_VERBOSE
	  if( UAVerb(infoVerb) ) {
	    G4cout << " GmUserSteppingActionList::UserSteppingAction " << (*aite)->GetName() << " FutureFilter " << (*ite)->GetName()  << " AcceptFilter? " << bAccept << G4endl;
	  }
#endif
	  if( bAccept ) {
	    //-	    stepMgr->SaveStep( aStep ); // it is not saved by GmFutureFilterUA yet
	    std::vector<G4Step*> steps = stepMgr->GetSteps( futureFilter, aStep );
	    std::vector<G4Step*>::const_iterator ites;
	    for( ites = steps.begin(); ites != steps.end(); ites++ ) {
#ifndef GAMOS_NO_VERBOSE
	  if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingActionList::UserSteppingAction futureFilter invoking action for step " << G4endl;
#endif
	      (*aite)->UserSteppingAction(*ites);
	    }
	  }
	
	  bAccept = FALSE; // do not invoke user action with current step
	  //-	  return; // avoid invoking user action, there no more filters anyhow

	}

      }

      if( bAccept ) {
#ifndef GAMOS_NO_VERBOSE
	if( UAVerb(infoVerb) ) G4cout << " Stepping action invoking " << (*aite)->GetName() << " " << theSteppingActions.size() << " " << *aite << G4endl;
#endif
	(*aite)->UserSteppingAction(aStep);
      }
      
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingActionList::UserSteppingAction futureFilter invoking action for step " << (*aite)->GetName() << G4endl;
#endif
      (*aite)->UserSteppingAction(aStep);
    }
  }

}


//-----------------------------------------------------------------------
void GmUserSteppingActionList::AddAction( GmUserSteppingAction* ua )
{
  theSteppingActions.push_back( ua );
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " GmUserSteppingActionList::AddAction " << ua << " N " << theSteppingActions.size() << G4endl;
#endif
  //--- first user action, register it to G4RunManager
  if( theSteppingActions.size() == 1) G4RunManager::GetRunManager()->SetUserAction( this );
  ua->SetSteppingManager( fpSteppingManager );
}

//-----------------------------------------------------------------------
void GmUserSteppingActionList::PutActionFirst(  GmUserSteppingAction* ua )
{
  vUSteppingA theSteppingActions2;
  vUSteppingA::const_iterator ite;
  theSteppingActions2.push_back( ua );
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ){
    if( *ite != ua ) theSteppingActions2.push_back(*ite);
  }

  theSteppingActions = theSteppingActions2;
}


//-----------------------------------------------------------------------
void GmUserSteppingActionList::RemoveAction(  GmUserSteppingAction* ua )
{
  vUSteppingA::iterator ite = find(theSteppingActions.begin(), theSteppingActions.end(), ua );
  if(ite != theSteppingActions.end() ) { 
    theSteppingActions.erase( ite );
    //--- No user actions in list, unregister it to G4RunManager
    if( theSteppingActions.size() == 0) G4RunManager::GetRunManager()->SetUserAction( (G4UserSteppingAction*)0 );
  } else {
    G4cerr << " !!WARNING UserSteppingActionaction is not in List " << ua->GetName() << G4endl;
  }
}


//-----------------------------------------------------------------------
GmUserSteppingAction* GmUserSteppingActionList::FindAction( const G4String& name) const
{
  vUSteppingA::const_iterator ite;
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      return *ite;
    }
  }

  return 0;

}
 

//------------------------------------------------------------------------
G4int GmUserSteppingActionList::CountActions( const G4String& name ) const
{
  G4int nAct = 0;
  vUSteppingA::const_iterator ite;
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      nAct++;
    }
  }
  
  return nAct;

}

//-----------------------------------------------------------------------
G4int GmUserSteppingActionList::DisableAction( const G4String& name)
{
  G4int nActions = 0;
  vUSteppingA::iterator ite;
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Disable();
      nActions++;
    }
  }

  return nActions;

}
//-----------------------------------------------------------------------
G4int GmUserSteppingActionList::EnableAction( const G4String& name)
{
  G4int nActions = 0;
  vUSteppingA::iterator ite;
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ) { 
    if( (*ite)->GetName() == name ) {
      (*ite)->Enable();
      nActions++;
    }
  }

  return nActions;

}
 
//-----------------------------------------------------------------------
void GmUserSteppingActionList::DumpActions( std::ostream& out, G4int enabled)
{
  G4String onoff[2] = {"OFF","ON"};
  vUSteppingA::iterator ite;
  for( ite = theSteppingActions.begin(); ite != theSteppingActions.end(); ite++ ) { 
    if( enabled == 0 
	|| (enabled == 1 && (*ite)->IsEnabled() )
	|| (enabled == -1 && !(*ite)->IsEnabled() ) ) {
      out << " " << (*ite)->GetName() << " " << onoff[ int((*ite)->IsEnabled()) ] << G4endl;
    }
 }

}
