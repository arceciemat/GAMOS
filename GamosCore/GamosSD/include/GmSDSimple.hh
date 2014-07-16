#ifndef GmSDSimple_HH
#define GmSDSimple_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;

enum SDPOS { SDPOS_Centre, SDPOS_Upper, SDPOS_Lower };
class GmSDSimple: public GmVSD
{

public:
  GmSDSimple(G4String type);
  ~GmSDSimple();

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

  GmDetUnitIDBuilderFromAncestors* GetDetUnitIDBuilder() const { return theDetUnitIDBuilder; }

protected:
  GmDetUnitIDBuilderFromAncestors* theDetUnitIDBuilder;
  SDPOS theSDPosType;
};

#endif
