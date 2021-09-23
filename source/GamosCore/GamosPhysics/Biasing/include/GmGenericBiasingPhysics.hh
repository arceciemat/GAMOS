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
//
// $Id: GmGenericBiasingPhysics.hh,v 1.6 2020/05/12 13:02:50 arce Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GmGenericBiasingPhysics_h
#define GmGenericBiasingPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include <vector>
class GmParameterMgr;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmGenericBiasingPhysics : public G4VPhysicsConstructor
{
public:
  
  GmGenericBiasingPhysics(const G4String& name = "BiasingP");
  virtual ~GmGenericBiasingPhysics();

public:
  // -- Used to select particles and processes to be under biasing:
  // ---- Put under biasing all physics processes of given particleName:
  void PhysicsBias(const G4String& particleName);
  // ---- Put under biasing processes in processToBiasNames of given particleName:
  void PhysicsBias(const G4String& particleName, const std::vector< G4String >& processToBiasNames);
  // ---- Allow for non physics biasing for particle:
  void NonPhysicsBias(const G4String& particleName);
  // ---- Put under biasing all physics processes and allow for non physics biasing:
  void Bias(const G4String& particleName);
  // ---- Put under biasing processes in processToBiasNames of given particleName:
  void Bias(const G4String& particleName, const std::vector< G4String >& processToBiasNames);

  // -- Bias groups of particles:
  // --  - particles which have been setup by names with above methods are not affected
  // --  - particles can be specified by PDG range
  // --  - particles can be specified by the charged ou neutral nature
  // --     - particles specified by name and PDG range are unaffected
  // -- Add a PDG range for particle to bias, anti-particles are included by default:
  void    PhysicsBiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle = true );
  void NonPhysicsBiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle = true );
  void           BiasAddPDGRange( G4int PDGlow, G4int PDGhigh, G4bool includeAntiParticle = true );
  // -- Will bias all charged particles:
  void    PhysicsBiasAllCharged( G4bool includeShortLived = false );
  void NonPhysicsBiasAllCharged( G4bool includeShortLived = false );
  void           BiasAllCharged( G4bool includeShortLived = false );
  // -- Will bias all neutral particles:
  void    PhysicsBiasAllNeutral( G4bool includeShortLived = false );
  void NonPhysicsBiasAllNeutral( G4bool includeShortLived = false );
  void           BiasAllNeutral( G4bool includeShortLived = false );


  // -- Information about biased particles:
  void BeVerbose() { fVerbose = true; }

  void DeReferenceWords(); //GAMOS
  
public:
  
  // This method is dummy for physics
  virtual void ConstructParticle();
  
  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type
  virtual void ConstructProcess();
  void AddSplittingProcess( std::vector<G4String> params);
  void CreateSplittingProcess( std::vector<G4String> params);

private:
  
  // hide assignment operator
  GmGenericBiasingPhysics & operator=(const GmGenericBiasingPhysics &right);
  GmGenericBiasingPhysics(const GmGenericBiasingPhysics&);

  // -- Particles under biasing:
  std::vector< G4String >  fBiasedParticles;
  std::vector< G4bool >   fBiasAllProcesses;
  // -- Related biased processes:
  std::vector< std::vector< G4String > > fBiasedProcesses;
  // -- non physics biased particles:
  std::vector< G4String > fNonPhysBiasedParticles;

  // -- Group of particles under biasing:
  std::vector< G4int >    fPhysBiasByPDGRangeLow,    fPhysBiasByPDGRangeHigh;
  std::vector< G4int > fNonPhysBiasByPDGRangeLow, fNonPhysBiasByPDGRangeHigh;
  G4bool fPhysBiasAllCharged, fNonPhysBiasAllCharged;
  G4bool fPhysBiasAllChargedISL, fNonPhysBiasAllChargedISL;
  G4bool fPhysBiasAllNeutral, fNonPhysBiasAllNeutral;
  G4bool fPhysBiasAllNeutralISL, fNonPhysBiasAllNeutralISL;
  
  // -- Report:
  G4bool fVerbose;

  std::vector<G4String> theSplittingProcParams;

  GmParameterMgr* theParamMgr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
