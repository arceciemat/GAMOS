#ifndef GmSDVirtSegmentedSphereRThetaPhi_HH
#define GmSDVirtSegmentedSphereRThetaPhi_HH

#include "GamosCore/GamosSD/include/GmSDVirtSegmentedSphereThetaPhi.hh"

class G4LogicalVolume;
class G4Step;

class GmSDVirtSegmentedSphereRThetaPhi: public GmSDVirtSegmentedSphereThetaPhi
{

public:
  GmSDVirtSegmentedSphereRThetaPhi(G4String type);
  ~GmSDVirtSegmentedSphereRThetaPhi();

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual unsigned long long GetDetUnitIDR( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

private:
  virtual void DefineSegmentation(const G4String& sdtype);
  virtual void DefineRSegmentation(const G4String& sdtype);
  
private:
  float theRLength;
};

#endif
