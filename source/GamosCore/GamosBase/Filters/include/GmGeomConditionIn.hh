#ifndef GmGeomConditionIn_h
#define GmGeomConditionIn_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////

#include "GmVGeomCondition.hh"
#include "G4TouchableHistory.hh"

class GmGeomConditionIn : public GmVGeomCondition
{
public: // with description
  GmGeomConditionIn();
  
  virtual ~GmGeomConditionIn(){delete theTouchable;};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;
  
};
#endif
