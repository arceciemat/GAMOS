#include "GmComputeDEDX.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ProcessManager.hh"

GmComputeDEDX::GmComputeDEDX()
{
}

GmComputeDEDX::~GmComputeDEDX()
{;}

G4double GmComputeDEDX::GetDEDX(G4Step* aStep, G4bool bRestricted)
{
  G4double dEdx = 0.0;
  
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4Material* mate = preStepPoint->GetMaterial();
  G4Region* region = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
  G4double kinEAver = (preStepPoint->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.;
  G4EmCalculator emCalculator;
  if( bRestricted ) {
    dEdx = emCalculator.GetDEDX(kinEAver,
				    part,
				    mate,
				    region);
    
  } else {
    // get Ioni process
    G4VEnergyLossProcess* ioniProcess = 0;
    G4ProcessManager* procMgr = part->GetProcessManager();
    G4ProcessVector* procList = procMgr->GetProcessList();
    for( size_t jj = 0; jj < procList->size(); jj++ ){
      //      G4cout << "GmComputeDEDX " << (*procList)[jj]->GetProcessName() << G4endl; //GDEB
      if( (*procList)[jj]->GetProcessName().find("Ioni") != G4String::npos ) {
	ioniProcess = dynamic_cast<G4VEnergyLossProcess*>((*procList)[jj]);	
	break;
      }
    }
    if( ioniProcess ) {
      G4PhysicsTable* theDEDXunRestrictedTable = ioniProcess->DEDXunRestrictedTable();
      G4cout << ioniProcess->GetProcessName() << " GmComputeDEDX theDEDXunRestrictedTable " << theDEDXunRestrictedTable << " " << ioniProcess->DEDXTable() << G4endl; //GDEB
      dEdx = ioniProcess->fFactor*(*theDEDXunRestrictedTable)[ioniProcess->basedCoupleIndex]->Value(kinEAver, ioniProcess->idxDEDX);
      if(kinEAver < ioniProcess->minKinEnergy) {
	dEdx *= std::sqrt(kinEAver/ioniProcess->minKinEnergy);
      }
      //      G4cout << " GmComputeDEDX DEDXunResT= " << dEdx << G4endl; 
    } else {
      dEdx = 0.;
    }    
  }
  

  return dEdx;
} 

G4double GmComputeDEDX::GetElectronicDEDX(G4Step* aStep, G4bool bRestricted)
{
  
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double kinEAver = (preStepPoint->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.;
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4Material* mate = preStepPoint->GetMaterial();
  G4Region* region = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
  G4ProductionCuts* prodCuts = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetProductionCuts();
 //  G4Region* region = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
  G4double cut = prodCuts->GetProductionCut("e-");
  if( !bRestricted ) {
    cut = DBL_MAX;
  }
  G4double let;
  G4EmCalculator emCalculator;
  /*  for( kinEAver = 0.001; kinEAver < 2.; kinEAver += 0.001 ) {
    G4cout << kinEAver << " lets " <<  emCalculator.GetDEDX(kinEAver, part, mate, region) << " " << emCalculator.ComputeElectronicDEDX( kinEAver, part, mate, DBL_MAX) << " " << emCalculator.ComputeElectronicDEDX( kinEAver, part, mate, cut ) <<" " << emCalculator.GetDEDX(kinEAver, part, mate, region)-emCalculator.ComputeElectronicDEDX( kinEAver, part, mate, cut ) << G4endl;
  }
  std::vector<double> dd;  sqrt(dd[33]);
  */
  
  if( !bRestricted ) {
    let = emCalculator.ComputeElectronicDEDX( kinEAver, part, mate, DBL_MAX);
    //-    let = emCalculator.ComputeElectronicDEDX( preStepPoint->GetKineticEnergy(), part, mate, DBL_MAX);
    
    /* gives the same as restricted because the 3 models for protons are
    - G4BetheBlochModel::ComputeDEDXPerVolume  
     G4double tmax      = MaxSecondaryEnergy(p, kineticEnergy); =
     // projectile formfactor limit energy loss 
     G4double cutEnergy = std::min(std::min(cut,tmax), tlimit);
     kineticEnergy = 200
     tmax = 0.47854559136406977
    - G4MuPairProductionModel::ComputeDEDXPerVolume
     if (cutEnergy <= minPairEnergy || kineticEnergy <= lowestKinEnergy)
     { return dedx; }
     lowestKinEnergy = 7506.176104000000
    - G4MuBremsstrahlungModel::ComputeDEDXPerVolume(
      if (kineticEnergy <= lowestKinEnergy) { return dedx; }
      lowestKinEnergy = 1000
    - G4BetheBlochModel::ComputeDEDXPerVolume(
     G4double tmax      = MaxSecondaryEnergy(p, kineticEnergy);
     // projectile formfactor limit energy loss 
     G4double cutEnergy = std::min(std::min(cut,tmax), tlimit);
     tmax = 0.48087456409273061
    */

  } else {
    let = emCalculator.ComputeElectronicDEDX( kinEAver, part, mate, cut);
  }
  //  G4cout << let << " GmComputeDEDX::GetElectronicDEDX cut " << cut << G4endl; //GDEB
  //  G4cout << let << " GmComputeDEDX2 " << kinEAver << " " << part->GetParticleName() << " " << mate->GetName() << " "<< cut<< G4endl;//GDEB
  //  G4EmCalculator emCal;
  // G4cout << emCal.ComputeElectronicDEDX( kinEAver, part, mate) << " NEW GmComputeDEDX2 " << kinEAver << " " << part->GetParticleName() << " " << mate->GetName() << " "<< cut<< G4endl;//GDEB

  return let;
}

G4double GmComputeDEDX::GetSecondaryElectronEnergy(G4Step* aStep)
{
  const std::vector<const G4Track*>* secos = aStep->GetSecondaryInCurrentStep();
  std::vector<const G4Track*>::const_iterator ite;
  G4double ESeco = 0.;
  for( ite = secos->begin(); ite != secos->end(); ite++ ) {
    //Get the PDG code of every secondaty particles in current step
    const G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGEncoding() == 11 ) { // only secondary electrons  
      ESeco += secoTrack->GetKineticEnergy();
    }
  }
  //  if( secos->size() != 0 ) G4cout << " GetSecondaryElectronEnergy " << secos->size() << " E= " << ESeco << G4endl; //GDEB
  
  return ESeco;
}
