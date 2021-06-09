#ifndef GmUserActionMgr_h
#define GmUserActionMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmUserActionMgr    
Author:      P. Arce
Changes:     10/01: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the G4UserAction's and allow load on demand of any UserAction  */ 

class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;

class GmUserAction;
class GmUserSteppingAction;
class GmUserTrackingAction;
class GmUserStackingAction;
class GmUserEventAction;
class GmUserRunAction;
#include <vector>
#include <map>
#include "GmUserRunActionList.hh"
#include "GmUserEventActionList.hh"
#include "GmUserStackingActionList.hh"
#include "GmUserTrackingActionList.hh"
#include "GmUserSteppingActionList.hh"
class G4RunManager;
#include "globals.hh"


//------------------------------------------------------------------------
class GmUserActionMgr 
{ 
 private:
  GmUserActionMgr(){ };

 public:
  ~GmUserActionMgr();

  //! Get the only instance 
  static GmUserActionMgr* GetInstance();

  //--- Register an action to the type list. It will not be added to G4RunManager (and therefore not executed) until it is added by calling AddAction method. 
  void RegisterSteppingAction( GmUserSteppingAction* userAction);
  void RegisterTrackingAction( GmUserTrackingAction* userAction);
  void RegisterStackingAction( GmUserStackingAction* userAction);
  void RegisterEventAction( GmUserEventAction* userAction);
  void RegisterRunAction( GmUserRunAction* userAction);

  //--- Add an action to the corresponding ActionList, so that it will be executed
  void AddUserAction( GmUserAction* userAction, G4int iorder = -1);

  //---- Dump the action list
  //  Enabled = 0: all 
  //  Enabled = 1: only Enabled ones
  //  Enabled = -1: only Disabled ones
  void DumpUserActionList( std::ostream& out = G4cout, G4int Enabled = 0);
  //--- Disable all the actions with a given name 
  void DisableUserAction( const G4String& name );
  //--- Enable all the actions with a given name 
  void EnableUserAction( const G4String& name );


  //--- GmUserStackingAction do not inherit from G4UserStackingAction, so some method is needed to access the G4StackManager
  G4StackManager* GetStackManager(){
    return theStackingActionList->getStackManager();
  }

  void CheckIfRepeated(  const G4String& name, G4bool bAcceptRep = TRUE );
  template <class T>
  void CheckIfRepeated( const T* actionList, const G4String& name, const G4String& msg, G4bool bAcceptRep = TRUE );
  G4int CountInstances( const G4String& name );

  GmUserRunActionList* GetRunActionList() const {
    return theRunActionList; }
  GmUserEventActionList* GetEventActionList() const {
    return theEventActionList; }
  GmUserStackingActionList* GetStackingActionList() const {
    return theStackingActionList; }
  GmUserTrackingActionList* GetTrackingActionList() const {
    return theTrackingActionList; }
  GmUserSteppingActionList* GetSteppingActionList() const {
    return theSteppingActionList; }

  void PutSteppingActionFirst( GmUserSteppingAction* UA );
  
private:
  void Initialise();

private:
  static GmUserActionMgr* theInstance;

  //--- Pointers to the action lists
  GmUserRunActionList* theRunActionList;
  GmUserEventActionList* theEventActionList;
  GmUserStackingActionList* theStackingActionList;
  GmUserTrackingActionList* theTrackingActionList;
  GmUserSteppingActionList* theSteppingActionList;

  G4RunManager* theG4RunManager;

};

#include "GmUserActionMgr.icc"

#endif
