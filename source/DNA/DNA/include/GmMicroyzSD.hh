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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software 
// shall cite the following Geant4-DNA collaboration publications:
// Phys. Med. 31 (2015) 861-874
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
/// \file GmMicroyzSD.hh
/// \brief Definition of the GmMicroyzSD class

#ifndef GmMicroyzSD_h
#define GmMicroyzSD_h 1

#include "G4VSensitiveDetector.hh"

#include "GmMicroyzHit.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
//class GmAnalysisMgr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Tracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with 
/// non zero energy deposit.

class GmMicroyzSD : public G4VSensitiveDetector
{
public:
  GmMicroyzSD(const G4String& name);
  virtual ~GmMicroyzSD();
  
  // methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  G4double GetSumY() const {
    return theSumY;
  }
  G4double GetSumY2() const {
    return theSumY2;
  }
  G4double GetSumY4() const {
    return theSumY4;
  }
  G4double GetSumZ() const {
    return theSumZ;
  }
  G4double GetSumZ2() const {
    return theSumZ2;
  }
  G4double GetSumZ4() const {
    return theSumZ4;
  }
  G4double GetSumWei() const {
    return theSumWei;
  }

private: 
  void BookHistos( G4double Eincident );
  
private:
  GmMicroyzHitsCollection* fHitsCollection;
  
  G4double theSphereRadius;
  G4double theVolumeChord;
  G4double theVolumeMass;
  G4bool bTextFile;
  std::ofstream *theFileOut;
  G4bool bHistos;
  G4bool bHistosInitialised;
  G4int theHistoNumber;
  GmAnalysisMgr* theAnaMgr;

  G4double theSumY;
  G4double theSumY2;
  G4double theSumY4;
  G4double theSumZ;
  G4double theSumZ2;
  G4double theSumZ4;
  G4double theSumWei;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
