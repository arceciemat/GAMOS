// GmUserRunActionList:
// Author: P.Arce 
//         15/11/2000
// Class to allow the use of several G4RunActions: 
//   This class is registered to G4RunManager.
//   Other G4RunAction classes are registered to this
//  and the BeginOfRunAction/EndOfRunAction method of this class just makes a 
//  loop to the BeginOfRunAction/EndOfRunAction methods of the classes registered to it

#ifndef GmUserRunActionList_h
#define GmUserRunActionList_h 1

#include "G4UserRunAction.hh"
class GmUserRunAction;
#include "globals.hh"
#include <vector>
#include <map>
class G4Run;

typedef std::vector<GmUserRunAction*> vURunA;

class GmUserRunActionList : public G4UserRunAction
{

 public:
  GmUserRunActionList();
  ~GmUserRunActionList();
  
 public:
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);
  virtual G4Run* GenerateRun();

 public:
  void AddAction( GmUserRunAction* ua );
  void RemoveAction( GmUserRunAction* ua );

  GmUserRunAction* FindAction( const G4String& name) const;
  G4int CountActions( const G4String& name ) const;

  int DisableAction( const G4String& name);
  int EnableAction( const G4String& name);

  void DumpActions( std::ostream& out, G4int enabled);

  vURunA GetRunActions() const {
    return theRunActions;
  }
  
 private:
  vURunA theRunActions;
};

#endif
