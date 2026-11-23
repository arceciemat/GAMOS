#ifndef GmGeomConditionStart_h
#define GmGeomConditionStart_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of entering particle (if in GeomBoundary)
// 
///////////////////////////////////////////////////////////////////////////////
#include "G4TouchableHistory.hh"
class G4Navigator;
#include "GmVGeomCondition.hh"

class GmGeomConditionStart : public GmVGeomCondition
{
public: // with description
  GmGeomConditionStart();
  
  virtual ~GmGeomConditionStart(){delete theTouchable;};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);
  virtual const G4VTouchable* GetTouchableFromStackedTrack(const G4Track* aTrack);
  
private:
  G4TouchableHistory* theTouchable;  
  G4Navigator* theNavigator;
};
#endif
