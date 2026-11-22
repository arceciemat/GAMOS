#ifndef GmSDVirtSegmBox_HH
#define GmSDVirtSegmBox_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class G4Box;
class G4VSolid;

class GmSDVirtSegmBox: public GmVSD
{

public:
  GmSDVirtSegmBox(G4String type);
  ~GmSDVirtSegmBox(){};
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* );
  void GetIDAndLocalPosition( unsigned long long& detUnitID, G4ThreeVector& localPos, G4ThreeVector position, const G4NavigationHistory* touchable, G4VSolid* solid );
  void GetNextVoxel( unsigned long long& detUnitID, G4ThreeVector& localPos, G4double& stepLength, const G4ThreeVector& localDir );
  void CreateHitVS( G4Step* aStep, G4double energy, G4ThreeVector position, G4double time, unsigned long long id );
  void UpdateHitVS( GmHit* hit, G4double energy, G4ThreeVector& position, G4double& time, G4Track* aTrack );
  
  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual void CalculateAndSetPosition( GmHit*, G4Step* ){
  };
  virtual unsigned long long GetDetUnitID( G4Step* ){
    return 0;  // it is never called indeed
  }
  
private:
  void StoreNewHit( G4double energy, G4double time, G4ThreeVector positionPre, unsigned long long detUnitIDPre, G4Step* aStep);

private:
  G4int theNDivX;
  G4int theNDivY;
  G4int theNDivZ;
  G4int theNDivXY;
  G4Box* theG4Box;
  G4double theWidthX;
  G4double theWidthY;
  G4double theWidthZ;
  G4double theOffsetX;
  G4double theOffsetY;
  G4double theOffsetZ;
  G4bool bOffsetsAreSet;
  G4int theMaxNVoxels;
  G4double theTolerance;
  G4bool bOffsetOnce;
  G4bool bOffsetOnceIsDone;

  G4int theNAncestors;
  G4int theNShift;

};

#endif
