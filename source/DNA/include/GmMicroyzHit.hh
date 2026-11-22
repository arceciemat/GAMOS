#ifndef GmMicroyzHit_h
#define GmMicroyzHit_h 1

#include "G4ThreeVector.hh"

///
/// It defines data members to store the trackID, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fEdep, fPos

class GmMicroyzHit 
{
public:
  GmMicroyzHit();
  GmMicroyzHit(const GmMicroyzHit&);
  virtual ~GmMicroyzHit();
  
  // operators
  const GmMicroyzHit& operator=(const GmMicroyzHit&);
  G4bool operator==(const GmMicroyzHit&) const;
  
  // methods from base class
  virtual void Draw();
  virtual void Print();
  void Print(std::ostream& os, G4bool bVerbose );
  
  // Set methods
  void SetTrackID  (G4int track)      { fTrackID = track; };
  void SetEdep     (G4double de)      { fEdep = de; };
  void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
//NEW
  void SetIncidentEnergy (G4double e)      { fIncidentEnergy = e; };
  //
  // Get methods
  G4int GetTrackID() const     { return fTrackID; };
  G4double GetEdep() const     { return fEdep; };
  G4ThreeVector GetPos() const { return fPos; };
  //NEW
  G4double GetIncidentEnergy() const { return fIncidentEnergy; };
  //
  
private:
  G4int         fTrackID;
  G4double      fEdep;
  G4ThreeVector fPos;
  //NEW
  G4double      fIncidentEnergy;
  //
};

#endif
