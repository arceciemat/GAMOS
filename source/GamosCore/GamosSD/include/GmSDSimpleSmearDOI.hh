#ifndef GmSDSimpleSmearDOI_HH
#define GmSDSimpleSmearDOI_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"
#include <map>

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;

class GmSDSimpleSmearDOI: public GmVSD
{

public:
  GmSDSimpleSmearDOI(const char* type);
  GmSDSimpleSmearDOI(G4String type);
  ~GmSDSimpleSmearDOI();

  virtual void UpdateHit( GmHit* hit, G4Step* aStep );
  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

  GmDetUnitIDBuilderFromAncestors* GetDetUnitIDBuilder() const { return theDetUnitIDBuilder; }

private:
  GmDetUnitIDBuilderFromAncestors* theDetUnitIDBuilder;
  G4double theDOIResol;
  std::map<GmHit*,G4double> theLastSmearing; 

};

#endif
