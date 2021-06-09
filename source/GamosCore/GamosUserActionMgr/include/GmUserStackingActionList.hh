// GmUserStackingActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4TrackingActions: 
//   This class is registered to G4TrackingManager.
//   Other G4TrackingAction classes are registered to this
//  and the PreUserTrackingAction/PostUserTrackingAction method of this class just makes a 
//  loop to the PreUserTrackingAction/PostUserTrackingAction methods of the classes registered to it

#ifndef GmUserStackingActionList_h
#define GmUserStackingActionList_h 1

#include "G4UserStackingAction.hh"
class GmUserStackingAction;
#include "globals.hh"
#include "G4StackManager.hh"
#include <vector>
#include <map>
class G4Stack;

typedef std::vector<GmUserStackingAction*> vUStackingA;


class GmUserStackingActionList : public G4UserStackingAction
{

 public:
  GmUserStackingActionList();
  ~GmUserStackingActionList();
  
 public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();
  
 public:
  void AddAction( GmUserStackingAction* ua  );
  void RemoveAction( GmUserStackingAction* ua  );

  GmUserStackingAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);

  //--- GmUserStackingAction do not inherit from G4UserStackingAction, os some method is needed to access the G4StackManager
  G4StackManager* getStackManager(){
    return stackManager;
  }
  
  void DumpActions( std::ostream& out, G4int enabled);

  vUStackingA GetStackingActions() const {
    return theStackingActions;
  }

 private:
  vUStackingA theStackingActions;
};

#endif
