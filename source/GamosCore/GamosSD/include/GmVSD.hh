#ifndef GmVSD_HH
#define GmVSD_HH

#include "G4VSensitiveDetector.hh"
#include <vector>
#include "globals.hh"
class GmHitsEventMgr;
class GmHitList;
class GmHit;

enum SDSegmType { SDSegm, SDvirtSegm, SDnoSegm };

class GmVSD: public G4VSensitiveDetector{
public:
  GmVSD(G4String name);
  ~GmVSD();

  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void ClearHits();
  void DrawAll();
  void PrintAll();

  virtual G4bool IsInterestingStep(G4Step*aStep);
  virtual G4double GetEnergyInSD( G4Step* aStep );

  //----- Get and Set methods
  static SDSegmType GetSDSegmType() {return theSDSegmType; }
  static void SetSDSegmType( SDSegmType typ ) { theSDSegmType = typ; }

  G4String GetType() const {return GetPathName(); }

  //  virtual std::vector<GmHit*> GetHitsInEvent() const { 
    //-    G4cout << this << "GetHitsInEvent() " << theHitsInEvent.size() << G4endl; 
  //    return theHitsInEvent; }

protected:
  virtual void CreateHit( G4Step* aStep, unsigned long long unitID );
  virtual void UpdateHit( GmHit* hit, G4Step* aStep );

private:
  virtual unsigned long long GetDetUnitID( G4Step* aStep ) = 0;
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep ) = 0;
  G4String CleanPathName( G4String name );

protected:
  std::vector<GmHit*> theHitsInEvent;
  GmHit* theCurrentHit;
  static SDSegmType theSDSegmType;

  static G4bool bFirstSDBegin;
  static G4bool bFirstSDEnd;

  G4int theEventID;

  GmHitsEventMgr* theHitsEventMgr;

  G4String theSDType;

  GmHitList* theHitList;
};

#endif
