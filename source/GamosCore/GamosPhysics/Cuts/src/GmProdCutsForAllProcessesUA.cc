#include "GmProdCutsForAllProcessesUA.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ProductionCuts.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

//----------------------------------------------------------------
GmProdCutsForAllProcessesUA::GmProdCutsForAllProcessesUA()
{
}

//----------------------------------------------------------------
void GmProdCutsForAllProcessesUA::BeginOfRunAction( const G4Run* )
{
  G4RToEConvForGamma* theR2EGamma = new G4RToEConvForGamma();
  theR2EGamma->SetVerboseLevel(0);
  G4RToEConvForElectron* theR2EElectron = new G4RToEConvForElectron();
  theR2EElectron->SetVerboseLevel(0);
  G4RToEConvForPositron* theR2EPositron = new G4RToEConvForPositron();
  theR2EPositron->SetVerboseLevel(0);

  G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    G4LogicalVolume* lv = *lvcite;
    G4ProductionCuts* prodCuts = lv->GetMaterialCutsCouple()->GetProductionCuts();
    G4Material* mate = lv->GetMaterial();
    G4double rcutGamma = prodCuts->GetProductionCut("gamma");
    G4double cutGamma = theR2EGamma->Convert(rcutGamma,mate);
    theGammaCuts[lv] = cutGamma;
    G4double rcutElectron = prodCuts->GetProductionCut("e-");
    G4double cutElectron = theR2EElectron->Convert(rcutElectron,mate);
    theElectronCuts[lv] = cutElectron;
    G4double rcutPositron = prodCuts->GetProductionCut("e+");
    G4double cutPositron = theR2EPositron->Convert(rcutPositron,mate);
    thePositronCuts[lv] = cutPositron;
  }

}

//----------------------------------------------------------------
G4ClassificationOfNewTrack GmProdCutsForAllProcessesUA::ClassifyNewTrack(const G4Track* aTrack)
{
  // get cut 
  G4VPhysicalVolume* pv = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (const G4ThreeVector*)0,false, true );

  G4LogicalVolume* lv = pv->GetLogicalVolume();

  G4ParticleDefinition* part = aTrack->GetDefinition();

  G4bool bBigEnergy = true;
  if( part == G4Gamma::Gamma() ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmProdCutsForAllProcessesUA gamma " <<  aTrack->GetKineticEnergy() << " " << theGammaCuts[lv] << G4endl;
#endif
    if( aTrack->GetKineticEnergy() < theGammaCuts[lv] ) {
      bBigEnergy = false;
    }
  } else if( part == G4Electron::Electron() ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmProdCutsForAllProcessesUA e- " <<  aTrack->GetKineticEnergy() << " " << theElectronCuts[lv] << G4endl;
#endif
    if( aTrack->GetKineticEnergy() < theElectronCuts[lv] ) {
      bBigEnergy = false;
    }
  } else if( part == G4Positron::Positron() ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmProdCutsForAllProcessesUA e+ " <<  aTrack->GetKineticEnergy() << " " << thePositronCuts[lv] << G4endl;
#endif
    if( aTrack->GetKineticEnergy() < thePositronCuts[lv] ) {
      bBigEnergy = false;
    }
  }

  if( bBigEnergy ) {
    return fUrgent;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmProdCutsForAllProcessesUA track killed " << part->GetParticleName() << " E= " << aTrack->GetKineticEnergy() << " lv " << lv->GetName() << G4endl;
#endif

    return fKill;
  }

}
