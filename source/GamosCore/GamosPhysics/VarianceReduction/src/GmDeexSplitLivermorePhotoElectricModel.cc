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
// $Id: GmDeexSplitLivermorePhotoElectricModel.cc,v 1.10 2013/10/27 20:09:13 arce Exp $
// GEANT4 tag $Name:  $
//
//
// Author: Sebastien Incerti
//         30 October 2008
//         on base of G4LowEnergyPhotoElectric developed by A.Forti and M.G.Pia
//
// History:
// --------
// 15 Apr 2009   V Ivanchenko Cleanup initialisation and generation of secondaries:
//                  - apply internal high-energy limit only in constructor 
//                  - do not apply low-energy limit (default is 0)
//                  - remove GetMeanFreePath method and table
//                  - simplify sampling of deexcitation by using cut in energy
//                  - added protection against numerical problem in energy sampling 
//                  - use G4ElementSelector
// 23 Oct 2009   L Pandola
//                  - atomic deexcitation managed via G4VEmModel::DeexcitationFlag() is 
//                    set as "true" (default would be false)
// 15 Mar 2010   L Pandola
//                  - removed methods to set explicitely fluorescence cuts.
//                  Main cuts from G4ProductionCutsTable are always used
// 26 Dec 2010   V Ivanchenko Load data tables only once to avoid memory leak
// 30 May 2011   A Mantero & V Ivanchenko Migration to model design for deexcitation
// 
#include "GmDeexSplitLivermorePhotoElectricModel.hh"
#include "G4LossTableManager.hh"
#include "G4Electron.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4CrossSectionHandler.hh"
#include "G4ShellData.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4VPhotoElectricAngularDistribution.hh"
#include "G4PhotoElectricAngularGeneratorSimple.hh"
#include "G4PhotoElectricAngularGeneratorSauterGavrila.hh"
#include "G4PhotoElectricAngularGeneratorPolarized.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4AtomicShell.hh"
#include "G4Gamma.hh"

#include "G4LPhysicsFreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDeexSplitLivermorePhotoElectricModel::GmDeexSplitLivermorePhotoElectricModel(
           const G4ParticleDefinition* part, const G4String& nam)
  : G4LivermorePhotoElectricModel(nam)
{
  theNSplit = G4int(GmParameterMgr::GetInstance()->GetNumericValue("DeexSplitPhotoElectric:NSplit",1));
  theNSplitInv = 1./theNSplit;
}

GmDeexSplitLivermorePhotoElectricModel::~GmDeexSplitLivermorePhotoElectricModel()
{
}

void 
GmDeexSplitLivermorePhotoElectricModel::SampleSecondariesPS(std::vector<G4DynamicParticle*>* fvect,
							    std::vector<G4double>& weightVect,
						 const G4MaterialCutsCouple* couple,
						 const G4DynamicParticle* aDynamicGamma,
						 G4double,
						 G4double,
				   const G4Track&)
{

  G4double gammaEnergy = aDynamicGamma->GetKineticEnergy();
  if (verboseLevel > 3) {
    G4cout << "G4LivermorePhotoElectricModel::SampleSecondaries() Egamma(keV)= "
	   << gammaEnergy/keV << G4endl;
  }
  
  // kill incident photon
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);   
 
  // Returns the normalized direction of the momentum
  G4ThreeVector photonDirection = aDynamicGamma->GetMomentumDirection(); 

  // Select randomly one element in the current material
  //G4cout << "Select random atom Egamma(keV)= " << gammaEnergy/keV << G4endl;
  const G4Element* elm = SelectRandomAtom(couple->GetMaterial(),theGamma,
					  gammaEnergy);
  G4int Z = G4lrint(elm->GetZ());

  // Select the ionised shell in the current atom according to shell 
  //   cross sections
  // G4cout << "Select random shell Z= " << Z << G4endl;

  if(Z >= maxZ) { Z = maxZ-1; }

  // element was not initialised
  if(!fCrossSection[Z]) {
    char* path = getenv("G4LEDATA");
    ReadData(Z, path);
    if(!fCrossSection[Z]) { 
      fParticleChange->ProposeLocalEnergyDeposit(gammaEnergy);
      return;
    }
  }
  
  // shell index
  size_t shellIdx = 0;
  size_t nn = fNShellsUsed[Z];

  if(nn > 1) {
    if(gammaEnergy >= (fParam[Z])[0]) {
      G4double x1 = 1.0/gammaEnergy;
      G4double x2 = x1*x1;
      G4double x3 = x2*x1;
      G4double x4 = x3*x1;
      G4int idx   = nn*6 - 4;
      // when do sampling common factors are not taken into account
      // so cross section is not real
      G4double cs0 = G4UniformRand()*((fParam[Z])[idx] + x1*(fParam[Z])[idx+1]
				      + x2*(fParam[Z])[idx+2] 
				      + x3*(fParam[Z])[idx+3] 
				      + x4*(fParam[Z])[idx+4]);
      for(shellIdx=0; shellIdx<nn; ++shellIdx) {
	idx = shellIdx*6 + 2;
	if(gammaEnergy > (fParam[Z])[idx-1]) {
	  G4double cs = (fParam[Z])[idx] + x1*(fParam[Z])[idx+1] 
	    + x2*(fParam[Z])[idx+2] + x3*(fParam[Z])[idx+3] 
	    + x4*(fParam[Z])[idx+4];
	  if(cs >= cs0) { break; }
	}
      }
      if(shellIdx >= nn) { shellIdx = nn-1; }

    } else {

      // when do sampling common factors are not taken into account
      // so cross section is not real
      G4double cs = G4UniformRand();

      if(gammaEnergy >= (fParam[Z])[1]) {
	cs *= (fCrossSection[Z])->Value(gammaEnergy);
      } else {
	cs *= (fCrossSectionLE[Z])->Value(gammaEnergy);
      }

      for(size_t j=0; j<nn; ++j) {
	shellIdx = (size_t)fShellCrossSection.GetComponentID(Z, j);
	if(gammaEnergy > (fParam[Z])[6*shellIdx+1]) {
	  cs -= fShellCrossSection.GetValueForComponent(Z, j, gammaEnergy);
	}
	if(cs <= 0.0 || j+1 == nn) { break; }
      }
    }
  }

  G4double bindingEnergy = (fParam[Z])[shellIdx*6 + 1];
  //G4cout << "Z= " << Z << " shellIdx= " << shellIdx 
  //       << " nShells= " << fNShells[Z] 
  //       << " Ebind(keV)= " << bindingEnergy/keV 
  //       << " Egamma(keV)= " << gammaEnergy/keV << G4endl;

  const G4AtomicShell* shell = 0;

  // no de-excitation from the last shell
  if(fDeexcitationActive && shellIdx + 1 < nn) {
    G4AtomicShellEnumerator as = G4AtomicShellEnumerator(shellIdx);
    shell = fAtomDeexcitation->GetAtomicShell(Z, as);
  }

  // If binding energy of the selected shell is larger than photon energy
  //    do not generate secondaries
  if(gammaEnergy < bindingEnergy) {
    fParticleChange->ProposeLocalEnergyDeposit(gammaEnergy);
    return;
  }

  // Primary outcoming electron
  G4double eKineticEnergy = gammaEnergy - bindingEnergy;
  G4double edep = bindingEnergy;

  // Calculate direction of the photoelectron
  G4ThreeVector electronDirection = 
    GetAngularDistribution()->SampleDirection(aDynamicGamma, 
					      eKineticEnergy,
					      shellIdx, 
					      couple->GetMaterial());

  // The electron is created 
  G4DynamicParticle* electron = new G4DynamicParticle (theElectron,
						       electronDirection,
						       eKineticEnergy);
  fvect->push_back(electron);

  // Sample deexcitation
  if(shell) {
    G4int index = couple->GetIndex();
    if(fAtomDeexcitation->CheckDeexcitationActiveRegion(index)) {
      for( int ii = 0; ii < theNSplit; ii++ ){
	size_t nbefore = fvect->size();
	
	fAtomDeexcitation->GenerateParticles(fvect, shell, Z, index);
	size_t nafter = fvect->size();
	if(nafter > nbefore) {
	  for (size_t j=nbefore; j<nafter; ++j) {
	    edep -= ((*fvect)[j])->GetKineticEnergy();
	  } 
	}
      }
    }
  }
  // energy balance - excitation energy left
  if(edep > 0.0) {
    fParticleChange->ProposeLocalEnergyDeposit(edep);
  }

}



