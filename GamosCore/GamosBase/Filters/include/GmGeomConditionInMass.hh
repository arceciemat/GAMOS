#ifndef GmGeomConditionInMass_h
#define GmGeomConditionInMass_h 1

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Return touchable of volume traverse, but always in mass geometry, even if there is a parallel world defined
// 
///////////////////////////////////////////////////////////////////////////////

#include "GmVGeomCondition.hh"
class G4TouchableHistory;

class GmGeomConditionInMass : public GmVGeomCondition
{
public: // with description
  GmGeomConditionInMass();
  
  virtual ~GmGeomConditionInMass(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*);
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*);

private:
  G4TouchableHistory* theTouchable;
  
};
#endif
