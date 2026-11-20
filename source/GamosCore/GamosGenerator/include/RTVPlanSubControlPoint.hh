// Serves to handle ScanSpot in RTIonPlanControlPoint. Only 1 subCP in RTPlanControlPoint
#ifndef RTVPlanSubControlPoint_H
#define RTVPlanSubControlPoint_H
#include "G4ThreeVector.hh"

class RTVPlanControlPoint;
class RTVPlanBeam;

struct RTBeamStateData {
  G4double PositionX = 0.;
  G4double PositionY = 0.;
  G4double PositionZ = 0.;
  G4double Energy = 0.;
  G4double RotAngleX = 0.;
  G4double RotAngleY = 0.;
  G4double RotAngleZ = 0.;
  G4double SpotSize = 0.;
  G4double SourceAxisDistanceX = 0.;
  G4double SourceAxisDistanceY = 0.;
  RTVPlanControlPoint* ControlPoint = nullptr;
  RTVPlanBeam* Beam = nullptr;
  G4ThreeVector IsocenterPosition;
  G4double LimitingDeviceAngle = 0.;
};

class RTVPlanSubControlPoint 
{
public:
  RTVPlanSubControlPoint();
  ~RTVPlanSubControlPoint(){}
 
  virtual RTBeamStateData GetBeamStateData() = 0;
  
  G4int GetID() const {
    return theID;
  }
  
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
  G4int theID;
  G4double theMeterset;
  RTVPlanControlPoint* theControlPoint;

};

#endif // RTVPlanSubControlPoint_H
