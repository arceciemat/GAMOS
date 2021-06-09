//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "GmEWPSPEEffectModel.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForGamma.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSPEEffectModel::GmEWPSPEEffectModel(const G4ParticleDefinition* part,
					 const G4String& nam)
  : G4PEEffectModel(part, nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSPEEffectModel::~GmEWPSPEEffectModel()
{
}

void GmEWPSPEEffectModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries,
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicGamma,
		    G4double energyCut,
		    G4double maxEnergy,
		    const G4Track& track)
{
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " GmEWPSPEEffectModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif
#endif

  G4double positionZ = track.GetPosition().z();
  G4bool bIsApplicable = IsApplicable(positionZ);   //PS<
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
#endif
    SampleSecondaries(secondaries, couple, aDynamicGamma, energyCut, maxEnergy);
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }   //PS>

  theWgt = track.GetWeight();  
  theNewNSplit = 1;     // only split fat photons in zones 2 and 3
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Original photon wgt " << theWgt << G4endl;
#endif
#endif

  //- See in which splitting zone is the particle and set some configurations
  idZone = GetZone(positionZ);
  if (idZone == PSZBeforeRR) {
    if (theWgt < 1.0) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Original photon is non-fat." << G4endl;
#endif
#endif
      theWgt = CheckGasNonGas(track);
      if (theWgt == 0.) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Killing photon, rejected by Russian Roul." << G4endl;
#endif
#endif
	fParticleChange->ProposeTrackStatus(fStopAndKill);
	return;
      } else {
	//	fParticleChange->ProposeParentWeight(wgt);
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: New wgt for (fat) photon " << theWgt << G4endl;
#endif
#endif
      }
    }
  } else {            // zones 2 and 3
    if (theWgt > 0.99) {
      theNewNSplit = theNSplit;
      theWgt *= theNSplitInv;
    }
  }   //PS>

  const G4Material* aMaterial = couple->GetMaterial();

  G4double energy = aDynamicGamma->GetKineticEnergy();
  G4ParticleMomentum PhotonDirection = aDynamicGamma->GetMomentumDirection();
  G4double bindingEnergy = 0.;

  //----- Loop to generate multiple secondaries
  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS
    // select randomly one element constituing the material.
    const G4Element* anElement = SelectRandomAtom(aMaterial,theGamma,energy);
    
    //
    // Photo electron
    //
    
    // Select atomic shell
    G4int nShells = anElement->GetNbOfAtomicShells();
    G4int i  = 0;  
    while ((i<nShells) && (energy<anElement->GetAtomicShell(i))) i++;
    
    G4double edep = energy;

    // no shell available
    if (i == nShells) return;
    
    bindingEnergy  = anElement->GetAtomicShell(i);
    G4double elecKineEnergy = energy - bindingEnergy;
    
    if (elecKineEnergy > fminimalEnergy)
    {
	// direction of the photo electron
      edep = bindingEnergy;
      G4ThreeVector elecDirection =
	GetAngularDistribution()->SampleDirection(aDynamicGamma,
                                                  elecKineEnergy,
                                                  i,
                                                  couple->GetMaterial());

      G4DynamicParticle* aParticle =
	new G4DynamicParticle(theElectron, elecDirection, elecKineEnergy);
      
      secondaries->push_back(aParticle);
      secondaryWeights.push_back(theWgt);
    }
  }
    
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);
  fParticleChange->ProposeLocalEnergyDeposit(bindingEnergy);
  
}
