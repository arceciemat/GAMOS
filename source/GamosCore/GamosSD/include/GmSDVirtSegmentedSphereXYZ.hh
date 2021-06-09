#ifndef GmSDVirtSegmentedSphereRThetaPhiZ_HH
#define GmSDVirtSegmentedSphereRThetaPhiZ_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;

class GmSDVirtSegmentedSphereRThetaPhiZ: public GmVSD{

public:
  GmSDVirtSegmentedSphereRThetaPhiZ(G4String type);
  ~GmSDVirtSegmentedSphereRThetaPhiZ();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

private:
  void DefineSegmentation();
  
private:
  G4LogicalVolume* theLogVol;
  float theRadius;
  float theZLength;
  float theThetaAngle;
  std::vector<float> thePhiAngles;
  G4int MAXNDIV;
};

#endif
