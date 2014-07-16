#include "GmEWPSLivermorePhotoElectricModel.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4ParticleChangeForGamma.hh"
#include "G4VCrossSectionHandler.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4VPhotoElectricAngularDistribution.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSLivermorePhotoElectricModel::GmEWPSLivermorePhotoElectricModel(const G4ParticleDefinition* part,
                                             const G4String& nam)
 :G4LivermorePhotoElectricModel(nam)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmEWPSLivermorePhotoElectricModel::~GmEWPSLivermorePhotoElectricModel()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void 
GmEWPSLivermorePhotoElectricModel::
SampleSecondariesPS(std::vector<G4DynamicParticle*>* secondaries,
		    std::vector<G4double>& secondaryWeights,
		    const G4MaterialCutsCouple* couple,
		    const G4DynamicParticle* aDynamicGamma,
		    G4double energyCut,
		    G4double maxEnergy,
		    const G4Track& track)
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " GmEWPSLivermorePhotoElectricModel::SampleSecondariesPS, start wgt " << track.GetWeight() << G4endl;
#endif
  /*
 //PS<
  G4double positionZ = track.GetPosition().z();
  G4bool bIsApplicable = IsApplicable(positionZ);  
  // Do splitting only above plane and for FAT electrons 
  if( !bIsApplicable ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSBetheHeitlerModel::SampleSecondariesPS DO NOT SPLIT : "
	   << " IsApplicable? " << bIsApplicable << G4endl;
#endif
    SampleSecondaries(secondaries, couple, aDynamicGamma, energyCut, maxEnergy);
    for( unsigned int ii = 0; ii < secondaries->size(); ii++ ) {
      secondaryWeights.push_back(1.);
    }
    return;
  }   

  theWgt = track.GetWeight();  
  theNewNSplit = 1;     // only split fat photons in zones 2 and 3
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Original photon wgt " << theWgt << G4endl;
#endif

  //- See in which splitting zone is the particle and set some configurations
  idZone = GetZone(positionZ);
  if (idZone == PSZBeforeRR) {
    if (theWgt < 1.0) {
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Original photon is non-fat." << G4endl;
#endif
      theWgt = CheckGasNonGas(track);
      if (theWgt == 0.) {
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: Killing photon, rejected by Russian Roul." << G4endl;
#endif
	fParticleChange->ProposeTrackStatus(fStopAndKill);
	return;
      } else {
	//	fParticleChange->ProposeParentWeight(theWgt);
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmEWPSPEEffectModel: New wgt for (fat) photon " << theWgt << G4endl;
#endif
      }
    }
  } else {            // zones 2 and 3
    if (theWgt > 0.99) {
      theNewNSplit = theNSplit;
      theWgt *= theNSplitInv;
    }
  }   //PS>

  // Fluorescence generated according to:
  // J. Stepanek ,"A program to determine the radiation spectra due to a single atomic
  // subshell ionisation by a particle or due to deexcitation or decay of radionuclides", 
  // Comp. Phys. Comm. 1206 pp 1-1-9 (1997)
 
  if (verboseLevel > 3)
    G4cout << "Calling SampleSecondaries() of GmEWPSLivermorePhotoElectricModel" << G4endl;

  G4double photonEnergy = aDynamicGamma->GetKineticEnergy();
  

  // low-energy gamma is absorpted by this process
  if (photonEnergy <= lowEnergyLimit)
    {
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy);
      return;
    }
 
  // Returns the normalized direction of the momentum
  G4ThreeVector photonDirection = aDynamicGamma->GetMomentumDirection(); 

  // Select randomly one element in the current material
  //  G4int Z = crossSectionHandler->SelectRandomAtom(couple,photonEnergy);
  const G4ParticleDefinition* particle =  aDynamicGamma->GetDefinition();

  //----- Loop to generate multiple secondaries
  for (G4int ii=0; ii<theNewNSplit; ii++) {   //PS

    const G4Element* elm = SelectRandomAtom(couple->GetMaterial(),particle,photonEnergy);
    G4int Z = (G4int)elm->GetZ();
    
    // Select the ionised shell in the current atom according to shell cross sections
    size_t shellIndex = shellCrossSectionHandler->SelectRandomShell(Z,photonEnergy);
    
    // Retrieve the corresponding identifier and binding energy of the selected shell
    const G4AtomicTransitionManager* transitionManager = G4AtomicTransitionManager::Instance();
    const G4AtomicShell* shell = transitionManager->Shell(Z,shellIndex);
    G4double bindingEnergy = shell->BindingEnergy();
    G4int shellId = shell->ShellId();
    
    // Primary outcoming electron
    G4double eKineticEnergy = photonEnergy - bindingEnergy;
    
    // There may be cases where the binding energy of the selected shell is > photon energy
    // In such cases do not generate secondaries
    if (eKineticEnergy > 0.)
      {
	// Calculate direction of the photoelectron
	G4ThreeVector gammaPolarization = aDynamicGamma->GetPolarization();
	G4ThreeVector electronDirection = 
	  fElectronAngularGenerator->GetPhotoElectronDirection(photonDirection,
							      eKineticEnergy,
							      gammaPolarization,
							      shellId);
	
	// The electron is created ...
	G4DynamicParticle* electron = new G4DynamicParticle (G4Electron::Electron(),
							     electronDirection,
							     eKineticEnergy);
	secondaries->push_back(electron);
	secondaryWeights.push_back(theWgt);
      }
    else
      {
	bindingEnergy = photonEnergy;
      }
    
  }
  // deexcitation //PS delete
  
  // kill incident photon
  fParticleChange->SetProposedKineticEnergy(0.);
  */
  fParticleChange->ProposeTrackStatus(fStopAndKill);   
}
