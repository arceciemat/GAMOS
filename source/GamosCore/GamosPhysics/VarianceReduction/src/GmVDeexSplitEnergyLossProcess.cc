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
// $Id: GmVDeexSplitEnergyLossProcess.cc,v 1.4 2013/10/27 20:09:13 arce Exp $
// GEANT4 tag $Name:  $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     GmVDeexSplitEnergyLossProcess
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 13-11-02 Minor fix - use normalised direction (V.Ivanchenko)
// 04-12-02 Minor change in PostStepDoIt (V.Ivanchenko)
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 26-12-02 Secondary production moved to derived classes (V.Ivanchenko)
// 04-01-03 Fix problem of very small steps for ions (V.Ivanchenko)
// 20-01-03 Migrade to cut per region (V.Ivanchenko)
// 24-01-03 Temporarily close a control on usage of couples (V.Ivanchenko)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 06-02-03 Add control on tmax in PostStepDoIt (V.Ivanchenko)
// 13-02-03 SubCutoffProcessors defined for regions (V.Ivanchenko)
// 15-02-03 Lambda table can be scaled (V.Ivanchenko)
// 17-02-03 Fix problem of store/restore tables (V.Ivanchenko)
// 18-02-03 Add control on CutCouple usage (V.Ivanchenko)
// 26-02-03 Simplify control on GenericIons (V.Ivanchenko)
// 06-03-03 Control on GenericIons using SubType + update verbose (V.Ivanchenko)
// 10-03-03 Add Ion registration (V.Ivanchenko)
// 22-03-03 Add Initialisation of cash (V.Ivanchenko)
// 26-03-03 Remove finalRange modification (V.Ivanchenko)
// 09-04-03 Fix problem of negative range limit for non integral (V.Ivanchenko)
// 26-04-03 Fix retrieve tables (V.Ivanchenko)
// 06-05-03 Set defalt finalRange = 1 mm (V.Ivanchenko)
// 12-05-03 Update range calculations + lowKinEnergy (V.Ivanchenko)
// 13-05-03 Add calculation of precise range (V.Ivanchenko)
// 23-05-03 Remove tracking cuts (V.Ivanchenko)
// 03-06-03 Fix initialisation problem for STD ionisation (V.Ivanchenko)
// 21-07-03 Add UpdateEmModel method (V.Ivanchenko)
// 03-11-03 Fix initialisation problem in RetrievePhysicsTable (V.Ivanchenko)
// 04-11-03 Add checks in RetrievePhysicsTable (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 21-01-04 Migrade to G4ParticleChangeForLoss (V.Ivanchenko)
// 27-02-04 Fix problem of loss in low presure gases, cleanup precise range
//          calculation, use functions ForLoss in AlongStepDoIt (V.Ivanchenko)
// 10-03-04 Fix a problem of Precise Range table (V.Ivanchenko)
// 19-03-04 Fix a problem energy below lowestKinEnergy (V.Ivanchenko)
// 31-03-04 Fix a problem of retrieve tables (V.Ivanchenko)
// 21-07-04 Check weather AtRest are active or not (V.Ivanchenko)
// 03-08-04 Add pointer of DEDX table to all processes (V.Ivanchenko)
// 06-08-04 Clear up names of member functions (V.Ivanchenko)
// 06-08-04 Clear up names of member functions (V.Ivanchenko)
// 27-08-04 Add NeedBuildTables method (V.Ivanchneko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivantchenko)
// 11-03-05 Shift verbose level by 1 (V.Ivantchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivantchenko)
// 11-04-05 Use MaxSecondaryEnergy from a model (V.Ivanchenko)
// 25-07-05 Add extra protection PostStep for non-integral mode (V.Ivanchenko)
// 12-08-05 Integral=false; SetStepFunction(0.2, 0.1*mm) (mma)
// 18-08-05 Return back both AlongStep and PostStep from 7.0 (V.Ivanchenko)
// 02-09-05 Default StepFunction 0.2 1 mm + integral (V.Ivanchenko)
// 04-09-05 default lambdaFactor 0.8 (V.Ivanchenko)
// 05-10-05 protection against 0 energy loss added (L.Urban)
// 17-10-05 protection above has been removed (L.Urban)
// 06-01-06 reset currentCouple when StepFunction is changed (V.Ivanchenko)
// 10-01-06 PreciseRange -> CSDARange (V.Ivantchenko)
// 18-01-06 Clean up subcutoff including recalculation of presafety (VI)
// 20-01-06 Introduce G4EmTableType and reducing number of methods (VI)
// 22-03-06 Add control on warning printout AlongStep (VI)
// 23-03-06 Use isIonisation flag (V.Ivanchenko)
// 07-06-06 Do not reflect AlongStep in subcutoff regime (V.Ivanchenko)
// 14-01-07 add SetEmModel(index) and SetFluctModel() (mma)
// 16-01-07 add IonisationTable and IonisationSubTable (V.Ivanchenko)
// 16-02-07 set linLossLimit=1.e-6 (V.Ivanchenko)
// 13-03-07 use SafetyHelper instead of navigator (V.Ivanchenko)
// 10-04-07 use unique SafetyHelper (V.Ivanchenko)
// 12-04-07 Add verbosity at destruction (V.Ivanchenko)
// 25-04-07 move initialisation of safety helper to BuildPhysicsTable (VI)
// 27-10-07 Virtual functions moved to source (V.Ivanchenko)
// 24-06-09 Removed hidden bin in G4PhysicsVector (V.Ivanchenko)
// 15-10-10 Fixed 4-momentum balance if deexcitation is active (L.Pandola)
//
// Class Description:
//
// It is the unified energy loss process it calculates the continuous
// energy loss for charged particles using a set of Energy Loss
// models valid for different energy regions. There are a possibility
// to create and access to dE/dx and range tables, or to calculate
// that information on fly.
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "GmVDeexSplitEnergyLossProcess.hh"
#include "G4ProcessManager.hh"
#include "G4LossTableManager.hh"
#include "G4LossTableBuilder.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VEmModel.hh"
#include "G4VEmFluctuationModel.hh"
#include "G4DataVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4VParticleChange.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"
#include "G4GenericIon.hh"
#include "G4ProductionCutsTable.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysicsTableHelper.hh"
#include "G4SafetyHelper.hh"
#include "G4TransportationManager.hh"
#include "G4EmConfigurator.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4EmBiasingManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmVDeexSplitEnergyLossProcess::GmVDeexSplitEnergyLossProcess(const G4String& name, 
					   G4ProcessType type)
: G4VEnergyLossProcess(name, type)
{

  theNSplit = G4int(GmParameterMgr::GetInstance()->GetNumericValue("DeexSplitIonisation:NSplit",1));
  theNSplitInv = 1./theNSplit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmVDeexSplitEnergyLossProcess::~GmVDeexSplitEnergyLossProcess()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* GmVDeexSplitEnergyLossProcess::AlongStepDoIt(const G4Track& track,
                                                       const G4Step& step)
{
  fParticleChange.InitializeForAlongStep(track);
  // The process has range table - calculate energy loss
  if(!IsIonisationProcess() || !currentModel->IsActive(preStepScaledEnergy)) {
    return &fParticleChange;
  }

  // Get the actual (true) Step length
  G4double length = step.GetStepLength();
  if(length <= 0.0) { return &fParticleChange; }
  G4double eloss  = 0.0;
 
  /*
  if(1 < verboseLevel) {
    const G4ParticleDefinition* d = track.GetParticleDefinition();
    G4cout << "AlongStepDoIt for "
           << GetProcessName() << " and particle "
           << d->GetParticleName()
           << "  eScaled(MeV)= " << preStepScaledEnergy/MeV
           << "  range(mm)= " << fRange/mm
           << "  s(mm)= " << length/mm
	   << "  rf= " << reduceFactor
           << "  q^2= " << chargeSqRatio
           << " md= " << d->GetPDGMass()
           << "  status= " << track.GetTrackStatus()
           << G4endl;
  }
  */

  const G4DynamicParticle* dynParticle = track.GetDynamicParticle();

  // define new weight for primary and secondaries
  G4double weight = fParticleChange.GetParentWeight();
  if(weightFlag) {
    weight /= biasFactor;
    fParticleChange.ProposeParentWeight(weight);
  }

  // stopping
  if (length >= fRange) {
    eloss = preStepKinEnergy;
    if (useDeexcitation) {
      for( int ii = 0; ii < theNSplit; ii++ ){
	size_t nbefore = scTracks.size();
	atomDeexcitation->AlongStepDeexcitation(scTracks, step, 
						eloss, currentCoupleIndex);
	size_t nafter = scTracks.size();
	if(nafter > nbefore) {
	  for (size_t ii=nbefore; ii<nafter; ++ii) {
	    scTracks[ii]->SetWeight(weight*theNSplitInv);
	  }
	}
      }
      FillSecondariesAlongStep(eloss, weight);
    }
    fParticleChange.SetProposedKineticEnergy(0.0);
    fParticleChange.ProposeLocalEnergyDeposit(eloss);
    return &fParticleChange;
  }

  // Short step
  eloss = GetDEDXForScaledEnergy(preStepScaledEnergy)*length;

  // Long step
  if(eloss > preStepKinEnergy*linLossLimit) {

    G4double x = GetScaledRangeForScaledEnergy(preStepScaledEnergy) 
      - length/reduceFactor;
    eloss = preStepKinEnergy - ScaledKinEnergyForLoss(x)/massRatio;
   
    /*
    if(-1 < verboseLevel) 
      G4cout << "Long STEP: rPre(mm)= " 
             << GetScaledRangeForScaledEnergy(preStepScaledEnergy)/mm
             << " rPost(mm)= " << x/mm
             << " ePre(MeV)= " << preStepScaledEnergy/MeV
             << " eloss(MeV)= " << eloss/MeV
             << " eloss0(MeV)= "
             << GetDEDXForScaledEnergy(preStepScaledEnergy)*length/MeV
	     << " lim(MeV)= " << preStepKinEnergy*linLossLimit/MeV
             << G4endl;
    */
  }

  /*   
  G4double eloss0 = eloss;
  if(-1 < verboseLevel ) {
    G4cout << "Before fluct: eloss(MeV)= " << eloss/MeV
           << " e-eloss= " << preStepKinEnergy-eloss
           << " step(mm)= " << length/mm
           << " range(mm)= " << fRange/mm
           << " fluct= " << lossFluctuationFlag
           << G4endl;
  }
  */

  G4double cut  = (*theCuts)[currentCoupleIndex];
  G4double esec = 0.0;

  // SubCutOff 
  if(useSubCutoff) {
    if(idxSCoffRegions[currentCoupleIndex]) {

      G4bool yes = false;
      G4StepPoint* prePoint = step.GetPreStepPoint();

      // Check boundary
      if(prePoint->GetStepStatus() == fGeomBoundary) { yes = true; }

      // Check PrePoint
      else {
	G4double preSafety  = prePoint->GetSafety();
	G4double rcut = currentCouple->GetProductionCuts()->GetProductionCut(1);

	// recompute presafety
        if(preSafety < rcut) {
	  preSafety = safetyHelper->ComputeSafety(prePoint->GetPosition());
	}

        if(preSafety < rcut) { yes = true; }

	// Check PostPoint
	else {
	  G4double postSafety = preSafety - length; 
	  if(postSafety < rcut) {
	    postSafety = 
	      safetyHelper->ComputeSafety(step.GetPostStepPoint()->GetPosition());
	    if(postSafety < rcut) { yes = true; }
	  }
	}
      }
  
      // Decided to start subcut sampling
      if(yes) {

        cut = (*theSubCuts)[currentCoupleIndex];
 	eloss -= GetSubDEDXForScaledEnergy(preStepScaledEnergy)*length;
	esec = SampleSubCutSecondaries(scTracks, step, 
				       currentModel,currentCoupleIndex);
	// add bremsstrahlung sampling
	/*
	if(nProcesses > 0) {
	  for(G4int i=0; i<nProcesses; ++i) {
	    (scProcesses[i])->SampleSubCutSecondaries(
		scTracks, step, (scProcesses[i])->
		SelectModelForMaterial(preStepKinEnergy, currentCoupleIndex),
		currentCoupleIndex);
	  }
	} 
	*/
      }   
    }
  }

  // Corrections, which cannot be tabulated
  if(isIon) {
    G4double eadd = 0.0;
    G4double eloss_before = eloss;
    currentModel->CorrectionsAlongStep(currentCouple, dynParticle, 
				       eloss, eadd, length);
    if(eloss < 0.0) { eloss = 0.5*eloss_before; }
  }

  // Sample fluctuations
  if (lossFluctuationFlag) {
    G4VEmFluctuationModel* fluc = currentModel->GetModelOfFluctuations();
    if(fluc && 
      (eloss + esec + lowestKinEnergy) < preStepKinEnergy) {

      G4double tmax = 
	std::min(currentModel->MaxSecondaryKinEnergy(dynParticle),cut);
      G4double emean = eloss;
      eloss = fluc->SampleFluctuations(currentMaterial,dynParticle,
				       tmax,length,emean);
      /*                            
      if(-1 < verboseLevel) 
      G4cout << "After fluct: eloss(MeV)= " << eloss/MeV
             << " fluc= " << (eloss-eloss0)/MeV
             << " ChargeSqRatio= " << chargeSqRatio
             << " massRatio= " << massRatio
             << " tmax= " << tmax
             << G4endl;
      */
    }
  }

  // deexcitation
  if (useDeexcitation) {
    G4double eloss_before = eloss;
    for( int ii = 0; ii < theNSplit; ii++ ){
      size_t nbefore = scTracks.size();
      atomDeexcitation->AlongStepDeexcitation(scTracks, step, 
					      eloss, currentCoupleIndex);
      size_t nafter = scTracks.size();
      if(nafter > nbefore) {
	for (size_t ii=nbefore; ii<nafter; ++ii) {
	  scTracks[ii]->SetWeight(weight*theNSplitInv);
	}
      }
    }
    esec += eloss_before - eloss;
  }

  // Energy balanse
  G4double finalT = preStepKinEnergy - eloss - esec;
  if (finalT <= lowestKinEnergy) {
    eloss += finalT;
    finalT = 0.0;
  } else if(isIon) {
    fParticleChange.SetProposedCharge(
      currentModel->GetParticleCharge(track.GetParticleDefinition(),
				      currentMaterial,finalT));
  }

  FillSecondariesAlongStep(eloss, weight);
  fParticleChange.SetProposedKineticEnergy(finalT);
  fParticleChange.ProposeLocalEnergyDeposit(eloss);

  /*  
  if(-1 < verboseLevel) {
    G4cout << "Final value eloss(MeV)= " << eloss/MeV
           << " preStepKinEnergy= " << preStepKinEnergy
           << " postStepKinEnergy= " << finalT
           << " lossFlag= " << lossFluctuationFlag
           << "  status= " << track.GetTrackStatus()
           << G4endl;
  }
  */  

  return &fParticleChange;
}

