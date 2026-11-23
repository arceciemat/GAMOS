#ifndef GmSDSeparateByTime_HH
#define GmSDSeparateByTime_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;
class GmHitsEventMgr;

class GmSDSeparateByTime: public GmVSD
{

public:
  GmSDSeparateByTime(G4String type);
  ~GmSDSeparateByTime();

  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

  GmDetUnitIDBuilderFromAncestors* GetDetUnitIDBuilder() const { return theDetUnitIDBuilder; }

private:
  GmDetUnitIDBuilderFromAncestors* theDetUnitIDBuilder;

  GmHitsEventMgr* theHitsEventMg;

  G4double theMinimalMeasuringTime;

  G4bool bDiscardBigTimes;
  G4bool bBigTimeCalculated;
  G4double theBigTimeFactor;
  G4double theBiggestHitTime;

};
#endif
