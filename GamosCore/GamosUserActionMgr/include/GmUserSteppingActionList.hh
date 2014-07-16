 // GmUserSteppingActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4SteppingActions: 
//   This class is registered to G4SteppingManager.
//   Other G4SteppingAction classes are registered to this
//  and the UserSteppingAction method of this class just makes a 
//  loop to the UserSteppingAction methods of the classes registered to it

#ifndef GmUserSteppingActionList_h
#define GmUserSteppingActionList_h 1

#include "G4UserSteppingAction.hh"
class GmUserSteppingAction;
#include "G4Timer.hh"
#include "globals.hh"
#include <vector>
#include <map>

class G4Step;

typedef std::vector<GmUserSteppingAction*> vUSteppingA;

class GmUserSteppingActionList : public G4UserSteppingAction
{
  //-  typedef map< G4String, GmUserAction*, less<G4String> > msUA;

 public:
  GmUserSteppingActionList();
  ~GmUserSteppingActionList();
  
 public:
  virtual void UserSteppingAction(const G4Step* aStep);
  
 public:
  void AddAction( GmUserSteppingAction* ua  );
  void RemoveAction( GmUserSteppingAction* ua  );

  GmUserSteppingAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);
  
  void DumpActions( std::ostream& out, G4int enabled);

  vUSteppingA GetSteppingActions() const {
    return theSteppingActions;
  }

  void PutActionFirst(  GmUserSteppingAction* ua );

 private:
  vUSteppingA theSteppingActions;
};

#endif
