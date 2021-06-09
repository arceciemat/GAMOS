#include "GmUserActionMgr.hh"
#include "GmUserActionMessenger.hh"

#include "GmUserRunActionList.hh"
#include "GmUserEventActionList.hh"
#include "GmUserStackingActionList.hh"
#include "GmUserTrackingActionList.hh"
#include "GmUserSteppingActionList.hh"
#include "GmUserRunAction.hh"
#include "GmUserEventAction.hh"
#include "GmUserStackingAction.hh"
#include "GmUserTrackingAction.hh"
#include "GmUserSteppingAction.hh"

#include "G4RunManager.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

GmUserActionMgr* GmUserActionMgr::theInstance = 0;

//------------------------------------------------------------------------
GmUserActionMgr::~GmUserActionMgr()
{
  if( theRunActionList ) delete theRunActionList;
  if( theEventActionList ) delete theEventActionList;
  if( theTrackingActionList ) delete theTrackingActionList;
  if( theStackingActionList ) delete theStackingActionList;
  if( theSteppingActionList ) delete theSteppingActionList;
  //delete all the actions
}


//------------------------------------------------------------------------
GmUserActionMgr* GmUserActionMgr::GetInstance()
{
 if( !theInstance ) {
    theInstance = new GmUserActionMgr();
    theInstance->Initialise();
    new GmUserActionMessenger( theInstance );
  }
  return theInstance;
}


//------------------------------------------------------------------------
void GmUserActionMgr::Initialise()
{
 
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(infoVerb) ) G4cout << " initialising GmUserActionMgr " << G4endl;
#endif

  theRunActionList = 0;
  theEventActionList = 0;
  theTrackingActionList = 0;
  theStackingActionList = 0;
  theSteppingActionList = 0;
  
  theG4RunManager = G4RunManager::GetRunManager();
}


//------------------------------------------------------------------------
void GmUserActionMgr::RegisterRunAction( GmUserRunAction* userAction)
{
  //--- add it to GmUserRunActionList 
  if( !theRunActionList ) {
    theRunActionList = new GmUserRunActionList;
  }

  theRunActionList->AddAction( userAction );

}

//------------------------------------------------------------------------
void GmUserActionMgr::RegisterEventAction( GmUserEventAction* userAction)
{
  //--- add it to GmUserEventActionList 
  if( !theEventActionList ) {
    theEventActionList = new GmUserEventActionList;
  }

  theEventActionList->AddAction( userAction );

}

//------------------------------------------------------------------------
void GmUserActionMgr::RegisterStackingAction( GmUserStackingAction* userAction)
{
  //--- add it to GmUserStackingActionList 
  if( !theStackingActionList ) {
    theStackingActionList = new GmUserStackingActionList;
  }

  theStackingActionList->AddAction( userAction );

}

//------------------------------------------------------------------------
void GmUserActionMgr::RegisterTrackingAction( GmUserTrackingAction* userAction)
{
  //--- add it to GmUserTrackingActionList 
  if( !theTrackingActionList ) {
    theTrackingActionList = new GmUserTrackingActionList;
  }

  theTrackingActionList->AddAction( userAction );

}

//------------------------------------------------------------------------
void GmUserActionMgr::RegisterSteppingAction( GmUserSteppingAction* userAction)
{
  //--- add it to GmUserSteppingActionList 
  if( !theSteppingActionList ) {
    theSteppingActionList = new GmUserSteppingActionList;
  }

  theSteppingActionList->AddAction( userAction );

}



//------------------------------------------------------------------------
void GmUserActionMgr::DisableUserAction( const G4String& name )
{
  //---- Several actions may share the same name: Loop to all the types of actions and delete the actions from the corresponding actionList (do not delete it from theRegistered actions, so you can Disable then later)
  G4int nActions = 0;
  if( theRunActionList ) nActions += theRunActionList->DisableAction( name );
  if( theEventActionList ) nActions += theEventActionList->DisableAction( name );
  if( theStackingActionList ) nActions += theStackingActionList->DisableAction( name );
  if( theTrackingActionList ) nActions += theTrackingActionList->DisableAction( name );
  if( theSteppingActionList ) nActions += theSteppingActionList->DisableAction( name );

  if( nActions == 0 ) {
    G4cerr << " WARNING: there is no action to Disable with name = " << name << G4endl;
  } 
}

//---------------------------------------------------------------------------
void GmUserActionMgr::EnableUserAction( const G4String& name )
{
  //---- Several actions may share the same name: Loop to all the types of actions and pass the actions to the corresponding actionList
  G4int nActions = 0;
  if( theRunActionList ) nActions += theRunActionList->EnableAction( name );
  if( theEventActionList ) nActions += theEventActionList->EnableAction( name );
  if( theStackingActionList ) nActions += theStackingActionList->EnableAction( name );
  if( theTrackingActionList ) nActions += theTrackingActionList->EnableAction( name );
  if( theSteppingActionList ) nActions += theSteppingActionList->EnableAction( name );

  if( nActions == 0 ) {
    G4cerr << " WARNING: there is no action to Enable with name = " << name << G4endl;
  }
}


//------------------------------------------------------------------------
void GmUserActionMgr::DumpUserActionList( std::ostream& out, G4int enabled)
{
  out << " ##### LIST OF ACTIONS " << G4endl;
  //----- Stepping Actions
  out << " ## RUN ACTIONS " << G4endl;
  if( theRunActionList ) theRunActionList->DumpActions( out, enabled );
  out << " ## EVENT ACTIONS " << G4endl;
  if( theEventActionList ) theEventActionList->DumpActions( out, enabled );
  out << " ## STACKING ACTIONS " << G4endl;
  if( theStackingActionList ) theStackingActionList->DumpActions( out, enabled );
  out << " ## TRACKING ACTIONS " << G4endl;
  if( theTrackingActionList ) theTrackingActionList->DumpActions( out, enabled );
  out << " ## STEPPING ACTIONS " << G4endl;
  if( theSteppingActionList ) theSteppingActionList->DumpActions( out, enabled );

}


//------------------------------------------------------------------------
void GmUserActionMgr::CheckIfRepeated( const G4String& name, G4bool bAcceptRep)
{
  if( theRunActionList ) CheckIfRepeated( theRunActionList, name, "Run", bAcceptRep );
  if( theEventActionList ) CheckIfRepeated( theEventActionList, name, "Event", bAcceptRep );
  if( theStackingActionList ) CheckIfRepeated( theStackingActionList, name, "Stacking", bAcceptRep );
  if( theTrackingActionList ) CheckIfRepeated( theTrackingActionList, name, "Tracking", bAcceptRep );
  if( theSteppingActionList ) CheckIfRepeated( theSteppingActionList, name, "Stepping", bAcceptRep );

}


//------------------------------------------------------------------------
G4int GmUserActionMgr::CountInstances( const G4String& name )
{
  //number of actions is the same in any list...
  G4int nAct = 0;
  if(theRunActionList) nAct = theRunActionList->CountActions(name);
  if( nAct > 0 ) return nAct;
  if(theEventActionList) nAct = theEventActionList->CountActions(name);
  if( nAct > 0 ) return nAct;
  if(theStackingActionList) nAct = theStackingActionList->CountActions(name);
  if( nAct > 0 ) return nAct;
  if(theTrackingActionList) nAct = theTrackingActionList->CountActions(name);
  if( nAct > 0 ) return nAct;
  if(theSteppingActionList) nAct = theSteppingActionList->CountActions(name);
  if( nAct > 0 ) return nAct;

  return 0;
}

void GmUserActionMgr::PutSteppingActionFirst( GmUserSteppingAction* UA )
{
  theSteppingActionList->PutActionFirst(UA);
}
