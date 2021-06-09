#ifndef GmSDSimpleExactPos_HH
#define GmSDSimpleExactPos_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;

class GmSDSimpleExactPos: public GmVSD
{

public:
  GmSDSimpleExactPos(const char* type);
  GmSDSimpleExactPos(G4String type);
  ~GmSDSimpleExactPos();

  virtual void UpdateHit( GmHit* hit, G4Step* aStep );
  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

  GmDetUnitIDBuilderFromAncestors* GetDetUnitIDBuilder() const { return theDetUnitIDBuilder; }

private:
  GmDetUnitIDBuilderFromAncestors* theDetUnitIDBuilder;
};

#endif
