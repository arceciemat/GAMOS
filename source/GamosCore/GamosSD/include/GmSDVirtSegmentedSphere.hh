#ifndef GmSDVirtSegmentedSphere_HH
#define GmSDVirtSegmentedSphere_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;

class GmSDVirtSegmentedSphere: public GmVSD{

public:
  GmSDVirtSegmentedSphere(G4String type);
  ~GmSDVirtSegmentedSphere();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep ) = 0;
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep ) = 0;

  void SetLogVol(G4LogicalVolume* lv ) {
    theLogVol = lv; }

public:
  virtual void DefineSegmentation(const G4String& sdtype) = 0;
  
protected:
  G4LogicalVolume* theLogVol;
  float theRadius;
  G4int MAXNDIV;

};

#endif
