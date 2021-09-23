#ifndef RTVPlanControlPoint_H
#define RTVPlanControlPoint_H
#include "G4ThreeVector.hh"
#include <map>
#include <iostream>

class RTVPlanBeam;

#include "RTPlanParamReader.hh"
#include "RTVPlanSubControlPoint.hh"

class RTVPlanControlPoint : public RTPlanParamReader
{
public:
  RTVPlanControlPoint();
  ~RTVPlanControlPoint(){}

  virtual RTBeamStateData GetBeamStateData(RTBeamStateData bsdata) = 0;

  G4bool operator==(const RTVPlanControlPoint& right) const;
    
  virtual void PlotBeamStates( G4double , G4double , G4double ){};
  G4int GetIndex() const {
    return theIndex;
  }

  std::vector<RTVPlanSubControlPoint*> GetSubCPs() const {
    return theSubCPs;
  }

  void SetMetersets( std::vector<RTVPlanSubControlPoint*> subCPs);
  
  RTVPlanBeam* GetBeam() const {
    return theBeam;
  }
  
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }

  G4double GetNominalBeamEnergy() const {
    return theNominalBeamEnergy;
  }
  G4double GetGantryAngle() const {
    return theGantryAngle;
  }
  G4double GetGantryPitchAngle() const {
    return theGantryPitchAngle;
  }
  G4double GetScanningSpotSize() const {
    return theScanningSpotSize;
  }
  G4int GetNumberOfPaintings() const {
    return theNumberOfPaintings;
  }
  G4double GetSourceAxisDistanceX() const {
    return theSourceAxisDistanceX;
  }
  G4double GetSourceAxisDistanceY() const {
    return theSourceAxisDistanceY;
  }

  virtual void RescaleMetersets()
  {}

protected:
  std::vector<RTVPlanSubControlPoint*> theSubCPs;
  //  G4ThreeVector theIsocenterPosition;
  G4double theNominalBeamEnergy;
  G4double theGantryAngle;
  G4double theGantryPitchAngle;
  G4double theScanningSpotSize;
  G4double theNumberOfPaintings;
  G4double theSourceAxisDistanceX;
  G4double theSourceAxisDistanceY;
  G4int theIndex;
  RTVPlanBeam* theBeam;
  
  G4String theFileName;
};

std::ostream& operator<<(std::ostream& out, const RTVPlanControlPoint& cp);


#endif // RTVPlanControlPoint_H
