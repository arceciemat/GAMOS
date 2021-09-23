#ifndef GmHit_HH
#define GmHit_HH

#include "globals.hh"
class GmEDepo;
class GmCheckOriginalGamma;
#include "G4TransportationManager.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <set>
#include <iostream>

class G4Step;
class G4Track;
class G4ParticleDefinition;

class GmHit
{

public:
  GmHit operator+=(const GmHit& hit);
  
  GmHit(){}
  GmHit(G4Step* aStep, G4double energy, unsigned long long id, const G4String& sdtyp, G4int evtid );
  GmHit( std::vector<G4String> wl );
  ~GmHit();

  G4bool CheckSameCell( unsigned long long idx );
  G4bool CheckSameCellAndTime( unsigned long long id, G4double time, G4double triggerTime, G4double measuringTime );

  void UpdateMe( G4Step* aStep, G4double enerResol );

  // Ahora un par de funciones virtuales
  void Draw();
  void Print(std::ostream& os = G4cout, G4bool bVerbose = TRUE);
  void Print(FILE* fout, G4bool bVerbose );
  void Build(std::vector<G4String>& wl );
  void PrintBadHit(std::vector<G4String>& wl );

  unsigned long long GetDetUnitID() const { return theDetUnitID;}
  G4int GetEventID() const { return theEventID; }
  G4double GetEnergy() const { return theEnergy;}
  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
  G4ThreeVector GetPosition() const { return thePosition;}
  std::set<G4int> GetOriginalTrackIDs() const { return theOriginalTrackIDs; }
  std::set<G4int> GetTrackIDs() const { return theTrackIDs;}
  std::vector<GmEDepo*> GetEDepos() const { return theEDepos; }
  void AddEDepo( GmEDepo* edep ) {
    theEDepos.push_back(edep); }
  G4int GetNEDepos() const { return theEDepos.size(); }
  G4String GetSDType() const { return theSDType; }

  void SetEnergy( G4double ener ) { theEnergy = ener;}
  void SetPosition(G4ThreeVector pos ); 
  void SetTimeMin( G4double time ) { theTimeMin = time;}
  void SetTimeMax( G4double time ) { theTimeMax = time;}

  void DeleteEDepoList();

  G4bool DeadTimeFound() const { return bDeadTimeFound; }
  void SetDeadTimeFound( G4bool dtf ){
    bDeadTimeFound = dtf; }
  
  void InsertTrackIDs( const G4Track* aTrack );

private:
  unsigned long long theDetUnitID;
  G4int theEventID;
  G4double theEnergy;
  G4double theTimeMin;
  G4double theTimeMax;
  G4ThreeVector thePosition;
  std::set<G4int> theOriginalTrackIDs;
  std::set<G4int> theTrackIDs;
  //  std::vector<G4int> theOriginalTrackIDs;
  //  std::vector<G4int> theTrackIDs;
  std::vector<GmEDepo*> theEDepos;
  G4String theSDType;
  G4ParticleDefinition* theParticle;

  GmCheckOriginalGamma* theCheckOriGamma;

  G4bool bDeadTimeFound;

  G4int theHitsTimeType;

};

std::ostream& operator<<(std::ostream& s, const GmHit& hit);

#endif
