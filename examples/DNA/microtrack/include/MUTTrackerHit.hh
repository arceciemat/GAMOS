//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

/// \file MUTTrackerHit.hh
/// \brief Definition of the MUTTrackerHit class

#ifndef MUTTrackerHit_h
#define MUTTrackerHit_h 1

#include "G4ParticleDefinition.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MUTTrackerHit : public G4VHit
{
  public:
    MUTTrackerHit();
    MUTTrackerHit(const MUTTrackerHit&);
    ~MUTTrackerHit() override;

    // operators
    const MUTTrackerHit& operator=(const MUTTrackerHit& right);
    G4bool operator==(const MUTTrackerHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // Methods from base class
    void Draw() override;
    void Print() override;

    // Set methods
    inline void SetTrackID(const G4int& track) { fTrackID = track; }
    inline void SetPosition(const G4ThreeVector& pos) { fPos = pos; }
    inline void SetEdep(const G4double& edep) { fEdep = edep; }
    inline void SetPartDef(const G4ParticleDefinition* partDef)
    {
      fPartDef = partDef;
    }

    // Get methods
    inline G4int GetTrackID() const { return fTrackID; }
    inline G4ThreeVector GetPosition() const { return fPos; }
    inline G4double GetEdep() const { return fEdep; }
    inline const G4ParticleDefinition* GetPartDef() const { return fPartDef; }

  private:
    G4int fTrackID = 0;  // Track ID
    G4ThreeVector fPos = G4ThreeVector();  // Position of hit
    G4double fEdep = 0.;  // Energy deposit
    const G4ParticleDefinition* fPartDef = nullptr;  // Particle definition
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<MUTTrackerHit> MUTTrackerHitColl;

extern G4ThreadLocal G4Allocator<MUTTrackerHit>* MUTTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* MUTTrackerHit::operator new(size_t)
{
  if (!MUTTrackerHitAllocator) MUTTrackerHitAllocator = new G4Allocator<MUTTrackerHit>;
  return (void*)MUTTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MUTTrackerHit::operator delete(void* hit)
{
  MUTTrackerHitAllocator->FreeSingle((MUTTrackerHit*)hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
