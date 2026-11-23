#ifndef RTIonPlanControlPoint_H
#define RTIonPlanControlPoint_H
#include <map>

class RTIonPlanSubControlPoint;
class RTIonPlanBeam;
#include "RTVPlanControlPoint.hh"

class RTIonPlanControlPoint : public RTVPlanControlPoint
{
public:
  RTIonPlanControlPoint( const G4String& fileName, RTIonPlanBeam* beam );
  ~RTIonPlanControlPoint(){}

  virtual RTBeamStateData GetBeamStateData(RTBeamStateData bsdata);

  G4double GetScanningSpotSize() const {
    return theScanningSpotSize;
  }

  /*  G4ThreeVector GetsocenterPosition() const {
    return theIsocenterPosition;
    }*/

  void PlotBeamStates( G4double minmaxX, G4double minmaxY, G4double maxMeterset);

  virtual void RescaleMetersets();

private:

  //-  std::vector<RTIonPlanSubControlPoint*> theSubCPs;
  //  G4ThreeVector theIsocenterPosition;
  G4double iCurrentPainting;
  //  RTIonPlanBeam* theBeam;
  G4double theSnoutPosition;
  
};

#endif // RTIonPlanControlPoint_H
