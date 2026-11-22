#ifndef GmGeomConditionTraverse_h
#define GmGeomConditionTraverse_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed, if not in GeomBoundary)
// 
///////////////////////////////////////////////////////////////////////////////
#include "G4TouchableHistory.hh"
#include "GmVGeomCondition.hh"

class GmGeomConditionTraverse : public GmVGeomCondition
{
public: // with description
  GmGeomConditionTraverse(){};
  
  virtual ~GmGeomConditionTraverse(){delete theTouchable;};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;  
};
#endif
