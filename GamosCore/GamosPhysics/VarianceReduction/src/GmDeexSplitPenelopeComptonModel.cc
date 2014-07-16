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
// $Id: GmDeexSplitPenelopeComptonModel.cc,v 1.4 2013/10/27 20:09:13 arce Exp $
// GEANT4 tag $Name:  $
//
// Author: Luciano Pandola
//
// History:
// --------
// 15 Feb 2010   L Pandola  Implementation
// 18 Mar 2010   L Pandola  Removed GetAtomsPerMolecule(), now demanded 
//                            to G4PenelopeOscillatorManager
// 01 Feb 2011   L Pandola  Suppress fake energy-violation warning when Auger is active.
//                          Make sure that fluorescence/Auger is generated only if 
//                          above threshold
// 24 May 2011   L Pandola  Renamed (make v2008 as default Penelope)
// 10 Jun 2011   L Pandola  Migrate atomic deexcitation interface
//

#include "GmDeexSplitPenelopeComptonModel.hh"
#include "G4ParticleDefinition.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4DynamicParticle.hh"
#include "G4VEMDataSet.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4AtomicShell.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4PenelopeOscillatorManager.hh"
#include "G4PenelopeOscillator.hh"
#include "G4LossTableManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


GmDeexSplitPenelopeComptonModel::GmDeexSplitPenelopeComptonModel(const G4ParticleDefinition* part,
					       const G4String& nam)
  : G4PenelopeComptonModel(part,nam)
{
  theNSplit = G4int(GmParameterMgr::GetInstance()->GetNumericValue("DeexSplitCompton:NSplit",1));
  theNSplitInv = 1./theNSplit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDeexSplitPenelopeComptonModel::~GmDeexSplitPenelopeComptonModel()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmDeexSplitPenelopeComptonModel::SampleSecondariesPS(std::vector<G4DynamicParticle*>* fvect,
							   std::vector<G4double>& weightVect,
					       const G4MaterialCutsCouple* couple,
					      const G4DynamicParticle* aDynamicGamma,
					      G4double,
					      G4double,
				   const G4Track&)
{
  
  // Penelope model v2008 to sample the Compton scattering final state.
  // D. Brusa et al., Nucl. Instrum. Meth. A 379 (1996) 167
  // The model determines also the original shell from which the electron is expelled, 
  // in order to produce fluorescence de-excitation (from G4DeexcitationManager)
  // 
  // The final state for Compton scattering is calculated according to the Klein-Nishina 
  // formula for energy > 5 MeV. In this case, the Doppler broadening is negligible and 
  // one can assume that the target electron is at rest.
  // For E < 5 MeV it is used the parametrization for the differential cross-section dSigma/dOmega,
  // to sample the scattering angle and the energy of the emerging electron, which is  
  // GmDeexSplitPenelopeComptonModel::DifferentialCrossSection(). The rejection method is 
  // used to sample cos(theta). The efficiency increases monotonically with photon energy and is 
  // nearly independent on the Z; typical values are 35%, 80% and 95% for 1 keV, 1 MeV and 10 MeV, 
  // respectively.
  // The parametrization includes the J(p) distribution profiles for the atomic shells, that are 
  // tabulated 
  // from Hartree-Fock calculations from F. Biggs et al., At. Data Nucl. Data Tables 16 (1975) 201. 
  // Doppler broadening is included.
  //

  if (verboseLevel > 3)
    G4cout << "Calling SampleSecondaries() of GmDeexSplitPenelopeComptonModel" << G4endl;
  
  G4double photonEnergy0 = aDynamicGamma->GetKineticEnergy();

  if (photonEnergy0 <= fIntrinsicLowEnergyLimit)
    {
      fParticleChange->ProposeTrackStatus(fStopAndKill);
      fParticleChange->SetProposedKineticEnergy(0.);
      fParticleChange->ProposeLocalEnergyDeposit(photonEnergy0);
      return ;
    }

  G4ParticleMomentum photonDirection0 = aDynamicGamma->GetMomentumDirection();
  const G4Material* material = couple->GetMaterial();

  G4PenelopeOscillatorTable* theTable = oscManager->GetOscillatorTableCompton(material); 

  const G4int nmax = 64;
  G4double rn[nmax]={0.0};
  G4double pac[nmax]={0.0};
  
  G4double S=0.0;
  G4double epsilon = 0.0;
  G4double cosTheta = 1.0;
  G4double hartreeFunc = 0.0;
  G4double oscStren = 0.0;
  size_t numberOfOscillators = theTable->size();
  size_t targetOscillator = 0;
  G4double ionEnergy = 0.0*eV;

  G4double ek = photonEnergy0/electron_mass_c2;
  G4double ek2 = 2.*ek+1.0;
  G4double eks = ek*ek;
  G4double ek1 = eks-ek2-1.0;

  G4double taumin = 1.0/ek2;
  G4double a1 = std::log(ek2);
  G4double a2 = a1+2.0*ek*(1.0+ek)/(ek2*ek2);

  G4double TST = 0;
  G4double tau = 0.;
 
  //If the incoming photon is above 5 MeV, the quicker approach based on the 
  //pure Klein-Nishina formula is used
  if (photonEnergy0 > 5*MeV)
    {
      do{
	do{
	  if ((a2*G4UniformRand()) < a1)
	    tau = std::pow(taumin,G4UniformRand());	    
	  else
	    tau = std::sqrt(1.0+G4UniformRand()*(taumin*taumin-1.0));	    
	  //rejection function
	  TST = (1.0+tau*(ek1+tau*(ek2+tau*eks)))/(eks*tau*(1.0+tau*tau));
	}while (G4UniformRand()> TST);
	epsilon=tau;
	cosTheta = 1.0 - (1.0-tau)/(ek*tau);

	//Target shell electrons	
	TST = oscManager->GetTotalZ(material)*G4UniformRand();
	targetOscillator = numberOfOscillators-1; //last level
	S=0.0;
	G4bool levelFound = false;
	for (size_t j=0;j<numberOfOscillators && !levelFound; j++)
	  {
	    S += (*theTable)[j]->GetOscillatorStrength();	    
	    if (S > TST) 
	      {
		targetOscillator = j;
		levelFound = true;
	      }
	  }
	//check whether the level is valid
	ionEnergy = (*theTable)[targetOscillator]->GetIonisationEnergy();
      }while((epsilon*photonEnergy0-photonEnergy0+ionEnergy) >0);
    }
  else //photonEnergy0 < 5 MeV
    {
      //Incoherent scattering function for theta=PI
      G4double s0=0.0;
      G4double pzomc=0.0;
      G4double rni=0.0;
      G4double aux=0.0;
      for (size_t i=0;i<numberOfOscillators;i++)
	{
	  ionEnergy = (*theTable)[i]->GetIonisationEnergy();
	  if (photonEnergy0 > ionEnergy)
	    {
	      G4double aux = photonEnergy0*(photonEnergy0-ionEnergy)*2.0;
	      hartreeFunc = (*theTable)[i]->GetHartreeFactor(); 
	      oscStren = (*theTable)[i]->GetOscillatorStrength();
	      pzomc = hartreeFunc*(aux-electron_mass_c2*ionEnergy)/
		(electron_mass_c2*std::sqrt(2.0*aux+ionEnergy*ionEnergy));
	      if (pzomc > 0) 	
		rni = 1.0-0.5*std::exp(0.5-(std::sqrt(0.5)+std::sqrt(2.0)*pzomc)*
				       (std::sqrt(0.5)+std::sqrt(2.0)*pzomc));		
	      else		  
		rni = 0.5*std::exp(0.5-(std::sqrt(0.5)-std::sqrt(2.0)*pzomc)*
				   (std::sqrt(0.5)-std::sqrt(2.0)*pzomc));	    
	      s0 += oscStren*rni;
	    }
	}      
      //Sampling tau
      G4double cdt1 = 0.;
      do
	{
	  if ((G4UniformRand()*a2) < a1)	    
	    tau = std::pow(taumin,G4UniformRand());	    
	  else	    
	    tau = std::sqrt(1.0+G4UniformRand()*(taumin*taumin-1.0));	    
	  cdt1 = (1.0-tau)/(ek*tau);
	  //Incoherent scattering function
	  S = 0.;
	  for (size_t i=0;i<numberOfOscillators;i++)
	    {
	      ionEnergy = (*theTable)[i]->GetIonisationEnergy();
	      if (photonEnergy0 > ionEnergy) //sum only on excitable levels
		{
		  aux = photonEnergy0*(photonEnergy0-ionEnergy)*cdt1;
		  hartreeFunc = (*theTable)[i]->GetHartreeFactor(); 
		  oscStren = (*theTable)[i]->GetOscillatorStrength();
		  pzomc = hartreeFunc*(aux-electron_mass_c2*ionEnergy)/
		    (electron_mass_c2*std::sqrt(2.0*aux+ionEnergy*ionEnergy));
		  if (pzomc > 0) 
		    rn[i] = 1.0-0.5*std::exp(0.5-(std::sqrt(0.5)+std::sqrt(2.0)*pzomc)*
					     (std::sqrt(0.5)+std::sqrt(2.0)*pzomc));		    
		  else		    
		    rn[i] = 0.5*std::exp(0.5-(std::sqrt(0.5)-std::sqrt(2.0)*pzomc)*
					 (std::sqrt(0.5)-std::sqrt(2.0)*pzomc));		    
		  S += oscStren*rn[i];
		  pac[i] = S;
		}
	      else
		pac[i] = S-1e-6;		
	    }
	  //Rejection function
	  TST = S*(1.0+tau*(ek1+tau*(ek2+tau*eks)))/(eks*tau*(1.0+tau*tau));  
	}while ((G4UniformRand()*s0) > TST);

      cosTheta = 1.0 - cdt1;
      G4double fpzmax=0.0,fpz=0.0;
      G4double A=0.0;
      //Target electron shell
      do
	{
	  do
	    {
	      TST = S*G4UniformRand();
	      targetOscillator = numberOfOscillators-1; //last level
	      G4bool levelFound = false;
	      for (size_t i=0;i<numberOfOscillators && !levelFound;i++)
		{
		  if (pac[i]>TST) 
		    {		     
		      targetOscillator = i;
		      levelFound = true;
		    }
		}
	      A = G4UniformRand()*rn[targetOscillator];
	      hartreeFunc = (*theTable)[targetOscillator]->GetHartreeFactor(); 
	      oscStren = (*theTable)[targetOscillator]->GetOscillatorStrength();
	      if (A < 0.5) 
		pzomc = (std::sqrt(0.5)-std::sqrt(0.5-std::log(2.0*A)))/
		  (std::sqrt(2.0)*hartreeFunc);	      
	      else		
		pzomc = (std::sqrt(0.5-std::log(2.0-2.0*A))-std::sqrt(0.5))/
		  (std::sqrt(2.0)*hartreeFunc);	
	    } while (pzomc < -1);

	  // F(EP) rejection
	  G4double XQC = 1.0+tau*(tau-2.0*cosTheta);
	  G4double AF = std::sqrt(XQC)*(1.0+tau*(tau-cosTheta)/XQC);
	  if (AF > 0) 
	    fpzmax = 1.0+AF*0.2;
	  else
	    fpzmax = 1.0-AF*0.2;	    
	  fpz = 1.0+AF*std::max(std::min(pzomc,0.2),-0.2);
	}while ((fpzmax*G4UniformRand())>fpz);
  
      //Energy of the scattered photon
      G4double T = pzomc*pzomc;
      G4double b1 = 1.0-T*tau*tau;
      G4double b2 = 1.0-T*tau*cosTheta;
      if (pzomc > 0.0)	
	epsilon = (tau/b1)*(b2+std::sqrt(std::abs(b2*b2-b1*(1.0-T))));	
      else	
	epsilon = (tau/b1)*(b2-std::sqrt(std::abs(b2*b2-b1*(1.0-T))));	
    } //energy < 5 MeV
  
  //Ok, the kinematics has been calculated.
  G4double sinTheta = std::sqrt(1-cosTheta*cosTheta);
  G4double phi = twopi * G4UniformRand() ;
  G4double dirx = sinTheta * std::cos(phi);
  G4double diry = sinTheta * std::sin(phi);
  G4double dirz = cosTheta ;

  // Update G4VParticleChange for the scattered photon
  G4ThreeVector photonDirection1(dirx,diry,dirz);
  photonDirection1.rotateUz(photonDirection0);
  fParticleChange->ProposeMomentumDirection(photonDirection1) ;

  G4double photonEnergy1 = epsilon * photonEnergy0;

  if (photonEnergy1 > 0.)  
    fParticleChange->SetProposedKineticEnergy(photonEnergy1) ;  
  else
  {
    fParticleChange->SetProposedKineticEnergy(0.) ;
    fParticleChange->ProposeTrackStatus(fStopAndKill);
  }
  
  //Create scattered electron
  G4double diffEnergy = photonEnergy0*(1-epsilon);
  ionEnergy = (*theTable)[targetOscillator]->GetIonisationEnergy();

  G4double Q2 = 
    photonEnergy0*photonEnergy0+photonEnergy1*(photonEnergy1-2.0*photonEnergy0*cosTheta);
  G4double cosThetaE = 0.; //scattering angle for the electron

  if (Q2 > 1.0e-12)    
    cosThetaE = (photonEnergy0-photonEnergy1*cosTheta)/std::sqrt(Q2);    
  else    
    cosThetaE = 1.0;    
  G4double sinThetaE = std::sqrt(1-cosThetaE*cosThetaE);

  //Now, try to handle fluorescence
  //Notice: merged levels are indicated with Z=0 and flag=30
  G4int shFlag = (*theTable)[targetOscillator]->GetShellFlag(); 
  G4int Z = (G4int) (*theTable)[targetOscillator]->GetParentZ();

  //initialize here, then check photons created by Atomic-Deexcitation, and the final state e-
  G4double bindingEnergy = 0.*eV;
  const G4AtomicShell* shell = 0;

  //Real level
  if (Z > 0 && shFlag<30)
    {
      shell = fTransitionManager->Shell(Z,shFlag-1);
      bindingEnergy = shell->BindingEnergy();    
    }

  G4double ionEnergyInPenelopeDatabase = ionEnergy;
  //protection against energy non-conservation
  ionEnergy = std::max(bindingEnergy,ionEnergyInPenelopeDatabase);  

  //subtract the excitation energy. If not emitted by fluorescence
  //the ionization energy is deposited as local energy deposition
  G4double eKineticEnergy = diffEnergy - ionEnergy; 
  G4double localEnergyDeposit = ionEnergy; 
  G4double energyInFluorescence = 0.; //testing purposes only
  G4double energyInAuger = 0; //testing purposes

  if (eKineticEnergy < 0) 
    {
      //It means that there was some problem/mismatch between the two databases. 
      //Try to make it work
      //In this case available Energy (diffEnergy) < ionEnergy
      //Full residual energy is deposited locally
      localEnergyDeposit = diffEnergy;
      eKineticEnergy = 0.0;
    }

  //the local energy deposit is what remains: part of this may be spent for fluorescence.
  //Notice: shell might be NULL (invalid!) if shFlag=30. Must be protected
  //Now, take care of fluorescence, if required
  if (fAtomDeexcitation && shell)
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
		    localEnergyDeposit -= itsEnergy * theNSplitInv;
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


  /*
  if(DeexcitationFlag() && Z > 5 && shellId>0) {

    const G4ProductionCutsTable* theCoupleTable=
      G4ProductionCutsTable::GetProductionCutsTable();

    size_t index = couple->GetIndex();
    G4double cutg = (*(theCoupleTable->GetEnergyCutsVector(0)))[index];
    G4double cute = (*(theCoupleTable->GetEnergyCutsVector(1)))[index];

    // Generation of fluorescence
    // Data in EADL are available only for Z > 5
    // Protection to avoid generating photons in the unphysical case of
    // shell binding energy > photon energy
    if (localEnergyDeposit > cutg || localEnergyDeposit > cute)
      { 
	G4DynamicParticle* aPhoton;
	deexcitationManager.SetCutForSecondaryPhotons(cutg);
	deexcitationManager.SetCutForAugerElectrons(cute);

	photonVector = deexcitationManager.GenerateParticles(Z,shellId);
	if(photonVector) 
	  {
	    size_t nPhotons = photonVector->size();
	    for (size_t k=0; k<nPhotons; k++)
	      {
		aPhoton = (*photonVector)[k];
		if (aPhoton)
		  {
		    G4double itsEnergy = aPhoton->GetKineticEnergy();
		    G4bool keepIt = false;
		    if (itsEnergy <= localEnergyDeposit)
		      {
			//check if good! 
			if(aPhoton->GetDefinition() == G4Gamma::Gamma()
			   && itsEnergy >= cutg)
			  {
			    keepIt = true;
			    energyInFluorescence += itsEnergy;			  
			  }
			if (aPhoton->GetDefinition() == G4Electron::Electron() && 
			    itsEnergy >= cute)
			  {
			    energyInAuger += itsEnergy;
			    keepIt = true;
			  }
		      }
		    //good secondary, register it
		    if (keepIt)
		      {
			localEnergyDeposit -= itsEnergy;
			fvect->push_back(aPhoton);
		      }		    
		    else
		      {
			delete aPhoton;
			(*photonVector)[k] = 0;
		      }
		  }
	      }
	    delete photonVector;
	  }
      }
  }
  */


  //Always produce explicitely the electron 
  G4DynamicParticle* electron = 0;

  G4double xEl = sinThetaE * std::cos(phi+pi); 
  G4double yEl = sinThetaE * std::sin(phi+pi);
  G4double zEl = cosThetaE;
  G4ThreeVector eDirection(xEl,yEl,zEl); //electron direction
  eDirection.rotateUz(photonDirection0);
  electron = new G4DynamicParticle (G4Electron::Electron(),
				    eDirection,eKineticEnergy) ;
  fvect->push_back(electron);
    

  if (localEnergyDeposit < 0)
    {
      G4cout << "WARNING-" 
	     << "GmDeexSplitPenelopeComptonModel::SampleSecondaries - Negative energy deposit"
	     << G4endl;
      localEnergyDeposit=0.;
    }
  fParticleChange->ProposeLocalEnergyDeposit(localEnergyDeposit);
  
  G4double electronEnergy = 0.;
  if (electron)
    electronEnergy = eKineticEnergy;
  if (verboseLevel > 1)
    {
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Energy balance from G4PenelopeCompton" << G4endl;
      G4cout << "Incoming photon energy: " << photonEnergy0/keV << " keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
      G4cout << "Scattered photon: " << photonEnergy1/keV << " keV" << G4endl;
      G4cout << "Scattered electron " << electronEnergy/keV << " keV" << G4endl;
      if (energyInFluorescence)
	G4cout << "Fluorescence x-rays: " << energyInFluorescence/keV << " keV" << G4endl;
      if (energyInAuger)
	G4cout << "Auger electrons: " << energyInAuger/keV << " keV" << G4endl;
      G4cout << "Local energy deposit " << localEnergyDeposit/keV << " keV" << G4endl;
      G4cout << "Total final state: " << (photonEnergy1+electronEnergy+energyInFluorescence+
					  localEnergyDeposit+energyInAuger)/keV << 
	" keV" << G4endl;
      G4cout << "-----------------------------------------------------------" << G4endl;
    }
  if (verboseLevel > 0)
    {
      G4double energyDiff = std::fabs(photonEnergy1+
				      electronEnergy+energyInFluorescence+
				      localEnergyDeposit+energyInAuger-photonEnergy0);
      if (energyDiff > 0.05*keV)
	G4cout << "Warning from G4PenelopeCompton: problem with energy conservation: " << 
	  (photonEnergy1+electronEnergy+energyInFluorescence+energyInAuger+localEnergyDeposit)/keV << 
	  " keV (final) vs. " << 
	  photonEnergy0/keV << " keV (initial)" << G4endl;
    }    
}

