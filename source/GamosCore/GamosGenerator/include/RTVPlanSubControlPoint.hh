#ifndef RTVPlanSubControlPoint_H
#define RTVPlanSubControlPoint_H
#include "G4ThreeVector.hh"
#include <map>
#include <vector>

class RTVPlanControlPoint;
class RTVPlanBeam;

struct RTBeamStateData {
  G4double PositionX;
  G4double PositionY;
  G4double PositionZ;
  G4double Energy;
  G4double RotAngleX;
  G4double RotAngleY;
  G4double RotAngleZ;
  G4double SpotSize;
  G4double SourceAxisDistanceX;
  G4double SourceAxisDistanceY;
  RTVPlanControlPoint* ControlPoint;
  RTVPlanBeam* Beam;
  G4ThreeVector IsocenterPosition;
  G4double LimitingDeviceAngle;
};

class RTVPlanSubControlPoint 
{
public:
  RTVPlanSubControlPoint();
  ~RTVPlanSubControlPoint(){}
 
  virtual RTBeamStateData GetBeamStateData() = 0;
  
  G4double GetMeterset() const {
    return theMeterset;
  }
  void SetMeterset( G4double ms ){
    theMeterset = ms;
  }

  RTVPlanControlPoint* GetControlPoint() const {
    return theControlPoint;
  }

  RTVPlanBeam* GetBeam() const;

protected:
  G4double theMeterset;
  RTVPlanControlPoint* theControlPoint;

};

#endif // RTVPlanSubControlPoint_H
