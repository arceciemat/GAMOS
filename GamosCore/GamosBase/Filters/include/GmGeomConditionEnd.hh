#ifndef GmGeomConditionEnd_h
#define GmGeomConditionEnd_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traverse if particle ends
// 
///////////////////////////////////////////////////////////////////////////////
class G4TouchableHistory;
#include "GmVGeomCondition.hh"

class GmGeomConditionEnd : public GmVGeomCondition
{
public: // with description
  GmGeomConditionEnd(){};
  
  virtual ~GmGeomConditionEnd(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;  
};
#endif
