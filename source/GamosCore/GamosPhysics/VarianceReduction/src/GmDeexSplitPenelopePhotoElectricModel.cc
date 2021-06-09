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
// $Id: GmDeexSplitPenelopePhotoElectricModel.cc,v 1.4 2012/12/15 20:42:22 arce Exp $
// GEANT4 tag $Name:  $
//
// Author: Luciano Pandola
//
// History:
// --------
// 08 Jan 2010   L Pandola  First implementation
// 01 Feb 2011   L Pandola  Suppress fake energy-violation warning when Auger is active.
//                          Make sure that fluorescence/Auger is generated only if 
//                          above threshold
// 25 May 2011   L Pandola  Renamed (make v2008 as default Penelope)
// 10 Jun 2011   L Pandola  Migrate atomic deexcitation interface
// 07 Oct 2011   L Pandola  Bug fix (potential violation of energy conservation)
//

#include "GmDeexSplitPenelopePhotoElectricModel.hh"
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4DynamicParticle.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsFreeVector.hh"
#include "G4ElementTable.hh"
#include "G4Element.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4AtomicShell.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4LossTableManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


GmDeexSplitPenelopePhotoElectricModel::GmDeexSplitPenelopePhotoElectricModel(const G4ParticleDefinition* part,
							   const G4String& nam)
  :G4PenelopePhotoElectricModel(part,nam)
{
  theNSplit = G4int(GmParameterMgr::GetInstance()->GetNumericValue("DeexSplitPhotoElectric:NSplit",1));
  theNSplitInv = 1./theNSplit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDeexSplitPenelopePhotoElectricModel::~GmDeexSplitPenelopePhotoElectricModel()
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmDeexSplitPenelopePhotoElectricModel::SampleSecondariesPS(std::vector<G4DynamicParticle*>* fvect,
							   std::vector<G4double>& weightVect,
						     const G4MaterialCutsCouple* couple,
						     const G4DynamicParticle* aDynamicGamma,
						     G4double,
						     G4double,
				   const G4Track&)
{
  //
  // Photoelectric effect, Penelope model v2008
  //
  // The target atom and the target shell are sampled according to the Livermore 
  // database 
  //  D.E. Cullen et al., Report UCRL-50400 (1989)
  // The angular distribution of the electron in the final state is sampled 
  // according to the Sauter distribution from 
  //  F. Sauter, Ann. Phys. 11 (1931) 454
  // The energy of the final electron is given by the initial photon energy minus 
  // the binding energy. Fluorescence de-excitation is subsequently produced 
  // (to fill the vacancy) according to the general Geant4 G4DeexcitationManager:
  //  J. Stepanek, Comp. Phys. Comm. 1206 pp 1-1-9 (1997)

  if (verboseLevel > 3)
    G4cout << "Calling SamplingSecondaries() of GmDeexSplitPenelopePhotoElectricModel" << G4endl;

  G4double photonEnergy = aDynamicGamma->GetKineticEnergy();

  // always kill primary
  fParticleChange->ProposeTrackStatus(fStopAndKill);
  fParticleChange->SetProposedKineticEnergy(0.);

  if (photonEnergy <= fIntrinsicLowEnergyLimit)
    {
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy);
      return ;
    }

  G4ParticleMomentum photonDirection = aDynamicGamma->GetMomentumDirection();

  // Select randomly one element in the current material
  if (verboseLevel > 2)
    G4cout << "Going to select element in " << couple->GetMaterial()->GetName() << G4endl;

  // atom can be selected efficiently if element selectors are initialised
  const G4Element* anElement =
    SelectRandomAtom(couple,G4Gamma::GammaDefinition(),photonEnergy);
  G4int Z = (G4int) anElement->GetZ();
  if (verboseLevel > 2)
    G4cout << "Selected " << anElement->GetName() << G4endl;
  
  // Select the ionised shell in the current atom according to shell cross sections
  //shellIndex = 0 --> K shell
  //             1-3 --> L shells
  //             4-8 --> M shells
  //             9 --> outer shells cumulatively
  //
  size_t shellIndex = SelectRandomShell(Z,photonEnergy);

  if (verboseLevel > 2)
    G4cout << "Selected shell " << shellIndex << " of element " << anElement->GetName() << G4endl;

  // Retrieve the corresponding identifier and binding energy of the selected shell
  const G4AtomicTransitionManager* transitionManager = G4AtomicTransitionManager::Instance();

  //The number of shell cross section possibly reported in the Penelope database 
  //might be different from the number of shells in the G4AtomicTransitionManager
  //(namely, Penelope may contain more shell, especially for very light elements).
  //In order to avoid a warning message from the G4AtomicTransitionManager, I 
  //add this protection. Results are anyway changed, because when G4AtomicTransitionManager
  //has a shellID>maxID, it sets the shellID to the last valid shell. 
  size_t numberOfShells = (size_t) transitionManager->NumberOfShells(Z);
  if (shellIndex >= numberOfShells)
    shellIndex = numberOfShells-1;

  const G4AtomicShell* shell = fTransitionManager->Shell(Z,shellIndex);
  G4double bindingEnergy = shell->BindingEnergy();
  //G4int shellId = shell->ShellId();

  //Penelope considers only K, L and M shells. Cross sections of outer shells are 
  //not included in the Penelope database. If SelectRandomShell() returns 
  //shellIndex = 9, it means that an outer shell was ionized. In this case the 
  //Penelope recipe is to set bindingEnergy = 0 (the energy is entirely assigned 
  //to the electron) and to disregard fluorescence.
  if (shellIndex == 9)
    bindingEnergy = 0.*eV;


  G4double localEnergyDeposit = 0.0;
  G4double cosTheta = 1.0;

  // Primary outcoming electron
  G4double eKineticEnergy = photonEnergy - bindingEnergy;
  
  // There may be cases where the binding energy of the selected shell is > photon energy
  // In such cases do not generate secondaries
  if (eKineticEnergy > 0.)
    {
      // The electron is created
      // Direction sampled from the Sauter distribution
      cosTheta = SampleElectronDirection(eKineticEnergy);
      G4double sinTheta = std::sqrt(1-cosTheta*cosTheta);
      G4double phi = twopi * G4UniformRand() ;
      G4double dirx = sinTheta * std::cos(phi);
      G4double diry = sinTheta * std::sin(phi);
      G4double dirz = cosTheta ;
      G4ThreeVector electronDirection(dirx,diry,dirz); //electron direction
      electronDirection.rotateUz(photonDirection);
      G4DynamicParticle* electron = new G4DynamicParticle (G4Electron::Electron(), 
							   electronDirection, 
							   eKineticEnergy);
      fvect->push_back(electron);
    } 
  else    
    bindingEnergy = photonEnergy;


  G4double energyInFluorescence = 0; //testing purposes
  G4double energyInAuger = 0; //testing purposes
 
  //Now, take care of fluorescence, if required. According to the Penelope
  //recipe, I have to skip fluoresence completely if shellIndex == 9 
  //(= sampling of a shell outer than K,L,M)
  if (fAtomDeexcitation && shellIndex<9)
    {      
      G4int index = couple->GetIndex();
      if (fAtomDeexcitation->CheckDeexcitationActiveRegion(index))
	{	
	  for( int ii = 0; ii < theNSplit; ii++ ){
	    size_t nBefore = fvect->size();
	    fAtomDeexcitation->GenerateParticles(fvect,shell,Z,index);
	    size_t nAfter = fvect->size();
	    
	    if (nAfter > nBefore) //actual production of fluorescence
	      {
		for (size_t j=nBefore;j<nAfter;j++) //loop on products
		  {
		    G4double itsEnergy = ((*fvect)[j])->GetKineticEnergy();
		    bindingEnergy -= itsEnergy * theNSplitInv;
		    if (((*fvect)[j])->GetParticleDefinition() == G4Gamma::Definition())
		      energyInFluorescence += itsEnergy * theNSplitInv;
		    else if (((*fvect)[j])->GetParticleDefinition() == G4Electron::Definition())
		      energyInAuger += itsEnergy * theNSplitInv;
		    weightVect.push_back(theNSplitInv);
		    
		  }
	      }
	    }

	}
    }

  //Residual energy is deposited locally
  localEnergyDeposit += bindingEnergy;
      
  if (localEnergyDeposit < 0)
    {
      G4cout << "WARNING - "
	     << "GmDeexSplitPenelopePhotoElectricModel::SampleSecondaries() - Negative energy deposit"
	     << G4endl;
      localEnergyDeposit = 0;
    }

  fParticleChange->ProposeLocalEnergyDeposit(localEnergyDeposit);

  if (verboseLevel > 1)
    {
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Energy balance from G4PenelopePhotoElectric" << G4endl;
      G4cout << "Selected shell: " << WriteTargetShell(shellIndex) << " of element " << 
	anElement->GetName() << G4endl;
      G4cout << "Incoming photon energy: " << photonEnergy/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
      if (eKineticEnergy)
	G4cout << "Outgoing electron " << eKineticEnergy/keV << " keV" << G4endl;
      if (energyInFluorescence)
	G4cout << "Fluorescence x-rays: " << energyInFluorescence/keV << " keV" << G4endl;
      if (energyInAuger)
	G4cout << "Auger electrons: " << energyInAuger/keV << " keV" << G4endl;
      G4cout << "Local energy deposit " << localEnergyDeposit/keV << " keV" << G4endl;
      G4cout << "Total final state: " << 
	(eKineticEnergy+energyInFluorescence+localEnergyDeposit+energyInAuger)/keV << 
	" keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
    }
  if (verboseLevel > 0)
    {
      G4double energyDiff = 
	std::fabs(eKineticEnergy+energyInFluorescence+localEnergyDeposit+energyInAuger-photonEnergy);
      if (energyDiff > 0.05*keV)
	{
	  G4cout << "Warning from G4PenelopePhotoElectric: problem with energy conservation: " << 
	    (eKineticEnergy+energyInFluorescence+localEnergyDeposit+energyInAuger)/keV 
		 << " keV (final) vs. " << 
	    photonEnergy/keV << " keV (initial)" << G4endl;
	  G4cout << "-----------------------------------------------------------" << G4endl;
	  G4cout << "Energy balance from G4PenelopePhotoElectric" << G4endl;
	  G4cout << "Selected shell: " << WriteTargetShell(shellIndex) << " of element " << 
	    anElement->GetName() << G4endl;
	  G4cout << "Incoming photon energy: " << photonEnergy/keV << " keV" << G4endl;
	  G4cout << "-----------------------------------------------------------" << G4endl;
	  if (eKineticEnergy)
	    G4cout << "Outgoing electron " << eKineticEnergy/keV << " keV" << G4endl;
	  if (energyInFluorescence)
	    G4cout << "Fluorescence x-rays: " << energyInFluorescence/keV << " keV" << G4endl;
	  if (energyInAuger)
	    G4cout << "Auger electrons: " << energyInAuger/keV << " keV" << G4endl;
	  G4cout << "Local energy deposit " << localEnergyDeposit/keV << " keV" << G4endl;
	  G4cout << "Total final state: " << 
	    (eKineticEnergy+energyInFluorescence+localEnergyDeposit+energyInAuger)/keV << 
	    " keV" << G4endl;
	  G4cout << "-----------------------------------------------------------" << G4endl;
	}
    }
}

