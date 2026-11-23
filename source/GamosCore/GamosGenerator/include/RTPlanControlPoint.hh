#ifndef RTPlanControlPoint_H
#define RTPlanControlPoint_H

#include "RTVPlanControlPoint.hh"
class RTPlanBeam;
class RTPlanSubControlPoint;

class RTPlanControlPoint : public RTVPlanControlPoint
{
public:
  RTPlanControlPoint( const G4String& fileName, RTPlanBeam* beam );
  ~RTPlanControlPoint(){}

  virtual RTBeamStateData GetBeamStateData(RTBeamStateData bsdata);

  /*  G4ThreeVector GetsocenterPosition() const {
    return theIsocenterPosition;
    }*/

private:
  //  RTPlanBeam* theBeam;
  G4double theLimitingDeviceAngle;
};

#endif // RTPlanControlPoint_H
