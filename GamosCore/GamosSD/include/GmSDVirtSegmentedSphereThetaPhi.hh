#ifndef GmSDVirtSegmentedSphereThetaPhi_HH
#define GmSDVirtSegmentedSphereThetaPhi_HH

#include "GamosCore/GamosSD/include/GmSDVirtSegmentedSphere.hh"

class G4LogicalVolume;
class G4Step;

class GmSDVirtSegmentedSphereThetaPhi: public GmSDVirtSegmentedSphere{

public:
  GmSDVirtSegmentedSphereThetaPhi(G4String type);
  ~GmSDVirtSegmentedSphereThetaPhi();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual unsigned long long GetDetUnitIDThetaPhi( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

public:
  virtual void DefineSegmentation(const G4String& sdtype);
  virtual void DefineThetaPhiSegmentation(const G4String& sdtype);
  
protected:
  float theThetaAngle;
  std::vector<float> thePhiAngles;

  G4double theRResolution;


};

#endif
