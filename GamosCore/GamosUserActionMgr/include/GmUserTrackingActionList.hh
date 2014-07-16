// GmUserTrackingActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4TrackingActions: 
//   This class is registered to G4TrackingManager.
//   Other G4TrackingAction classes are registered to this
//  and the PreUserTrackingAction/PostUserTrackingAction method of this class just makes a 
//  loop to the PreUserTrackingAction/PostUserTrackingAction methods of the classes registered to it

#ifndef GmUserTrackingActionList_h
#define GmUserTrackingActionList_h 1

#include "G4UserTrackingAction.hh"
class GmUserTrackingAction;
#include "globals.hh"
#include <vector>
#include <map>
class G4Track;

typedef std::vector<GmUserTrackingAction*> vUTrackingA;

class GmUserTrackingActionList : public G4UserTrackingAction
{

 public:
  GmUserTrackingActionList();
  ~GmUserTrackingActionList();
  
 public:
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  
 public:
  void AddAction( GmUserTrackingAction* ua );
  void RemoveAction( GmUserTrackingAction* ua );

  GmUserTrackingAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);

  void DumpActions( std::ostream& out, G4int enabled);
  
  vUTrackingA GetTrackingActions() const {
    return theTrackingActions;
  }

 private:
  vUTrackingA theTrackingActions;
};

#endif
