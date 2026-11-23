#ifndef GmInheritTrackInfoUA_hh
#define GmInheritTrackInfoUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <vector>
class  G4SteppingManager;

class GmInheritTrackInfoUA : public GmUserRunAction,
			     public GmUserSteppingAction
{
public:
  GmInheritTrackInfoUA();
  ~GmInheritTrackInfoUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction(const G4Step* );

  void SetTrackInfoName( const G4String name ) {
    theTrackInfoName = name;
  }

  void AddBoolValueName( const G4String name ) {
    theBoolValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddIntValueName( const G4String name ) {
    theIntValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddDoubleValueName( const G4String name ) {
    theDoubleValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddStringValueName( const G4String name ) {
    theStringValueNames.push_back(name);
    bSomethingToInherit = true;
  }
  void AddThreeVectorValueName( const G4String name ) {
    theThreeVectorValueNames.push_back(name);
    bSomethingToInherit = true;
  }

private:
  std::vector<G4String> theBoolValueNames;
  std::vector<G4String> theIntValueNames;
  std::vector<G4String> theDoubleValueNames;
  std::vector<G4String> theStringValueNames;
  std::vector<G4String> theThreeVectorValueNames;

  G4String theTrackInfoName;

  G4bool bSomethingToInherit;

  G4SteppingManager* theSteppingManager;
};

#endif
