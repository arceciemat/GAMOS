#ifndef GmGeomConditionIn_h
#define GmGeomConditionIn_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traversed
// 
///////////////////////////////////////////////////////////////////////////////

#include "GmVGeomCondition.hh"
class G4TouchableHistory;

class GmGeomConditionIn : public GmVGeomCondition
{
public: // with description
  GmGeomConditionIn();
  
  virtual ~GmGeomConditionIn(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;
  
};
#endif
