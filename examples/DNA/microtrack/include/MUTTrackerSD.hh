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

/// \file MUTTrackerSD.hh
/// \brief Definition of the MUTTrackerSD class

#ifndef MUTTrackerSD_h
#define MUTTrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "MUTTrackerHit.hh"

#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
class GmAnalysisMgr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MUTTrackerSD : public G4VSensitiveDetector,
		     public GmVHistoBuilder
{
  public:
  MUTTrackerSD(G4String sdName);
    ~MUTTrackerSD() override;

    // methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    // Set methods
    inline void SetParID(const G4int& parID) { fParID = parID; }

    // Get methods
    inline G4int GetParID() const { return fParID; }

  private:
  void InitialiseHistoNames();
  void BookHistos();

private :
  MUTTrackerHitColl* fHitsCollection = nullptr;
  G4int fParID = 0;  // Particle ID

  GmAnalysisMgr* theAnaMgr;
  G4bool bDetectorDataInitialised;
  G4double theHitSelRegXY;
  G4double theHitSelRegZ;
  G4double theDetDensity;
  G4double theSiteRadius;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
