// GmUserAction:
// Author: P.Arce  02/03/2002
// Base class for all G4 user actions
// Holds the name and group of user actions
// Keeps and empty method start/end run, start/end event, start/end track and stepping for calling all user actions with a pointer of this class type

#ifndef GmUserAction_h
#define GmUserAction_h 1

#include "globals.hh"
class G4Run;
class G4Event;
class G4Track;
class G4Step;
#include "G4ClassificationOfNewTrack.hh"
#include "globals.hh"
class GmVFilter;
class GmVClassifier;
#include <vector>

class GmUserAction
{
 protected:
  GmUserAction();
 public:
  // public:
  GmUserAction( const G4String& name );
  virtual ~GmUserAction();

 public:
  virtual void InitialiseHistoNames(){
  }

  const G4String GetName() const{ 
    return theName; 
  }
  bool IsEnabled() const{
    return isEnabled; 
  }

  void SetFilterOrClassifier(const G4String& objectName);

  //  virtual void DefineName( const G4String& Name ) = 0;

  void SetName( const G4String& Name ){
    theName = Name; 
  }
  void Disable() {
    G4cout << " disabling " << this << G4endl;
    isEnabled = FALSE; 
  }
  void Enable() {
    isEnabled = TRUE; 
  }

  G4bool ApplyFiltersToStepping() const {
    return bApplyFiltersToStepping; 
  }
  G4bool ApplyFiltersToPreTracking() const {
    return bApplyFiltersToPreTracking; 
  }
  G4bool ApplyFiltersToPostTracking() const {
    return bApplyFiltersToPostTracking; 
  }
  G4bool ApplyFiltersToStacking() const {
    return bApplyFiltersToStacking; 
  }
  void SetApplyFiltersToStepping(G4bool af ) {
    bApplyFiltersToStepping = af; 
  }
  G4bool CheckAllFiltersAtStepping() const {
    return bCheckAllFiltersAtStepping; 
  }
  G4bool CheckAllFiltersAtPreTracking() const {
    return bCheckAllFiltersAtPreTracking; 
  }
  G4bool CheckAllFiltersAtPostTracking() const {
    return bCheckAllFiltersAtPostTracking; 
  }
  G4bool CheckAllFiltersAtStacking() const {
    return bCheckAllFiltersAtStacking; 
  }

public:
  std::vector<GmVFilter*> theFilters;
  GmVClassifier* theClassifier;
protected:
  G4String theName;
  G4bool isEnabled;

  G4bool bApplyFiltersToStepping;
  G4bool bApplyFiltersToPreTracking;
  G4bool bApplyFiltersToPostTracking;
  G4bool bApplyFiltersToStacking;

  G4bool bCheckAllFiltersAtStepping;
  G4bool bCheckAllFiltersAtPreTracking;
  G4bool bCheckAllFiltersAtPostTracking;
  G4bool bCheckAllFiltersAtStacking;

};
#endif
