#include "GmPrintEMParamsUA.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4VProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4ProcessVector.hh"
#include "G4VMscModel.hh"
#include "G4VMultipleScattering.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPrintEMParamsUA::GmPrintEMParamsUA()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPrintEMParamsUA::~GmPrintEMParamsUA()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmPrintEMParamsUA::BeginOfRunAction(const G4Run*)
{  

  G4cout << "###### EM PARAMETERS from G4EmParameters ##### " << G4endl;
  G4EmParameters* param = G4EmParameters::Instance();

  param->Dump();

  G4cout << "###### EM PARAMETERS ##### " << G4endl;
							
  msmsd theParams = GetIoniParams();
  msmsd::const_iterator ite;
  std::map<G4String,double>::const_iterator ite1;
  for( ite = theParams.begin(); ite != theParams.end(); ite++ ) {
    std::map<G4String,double>* Params = (*ite).second;
    for( ite1 = Params->begin(); ite1 != Params->end(); ite1++ ) {
      G4cout << "# EM PARAM # " << (*ite).first << " " << (*ite1).first << " : " << (*ite1).second << G4endl;
    }
  }

  G4cout << "# EM PARAM # ELoss Fluctuation : " << param->LossFluctuation() << G4endl; 
  G4cout << "# EM PARAM # ELoss BuildCSDARange : " << param->BuildCSDARange() << G4endl; 
  G4cout << "# EM PARAM # ELoss LinearLossLimit : " << param->LinearLossLimit() << G4endl; 
  G4cout << "# EM PARAM # ELoss MinKinEnergy : " << param->MinKinEnergy() << G4endl; 
  G4cout << "# EM PARAM # ELoss MaxKinEnergy : " << param->MaxKinEnergy() << G4endl; 
  G4cout << "# EM PARAM # ELoss NumberOfBins : " << param->NumberOfBins() << G4endl; 
  G4cout << "# EM PARAM # NumberOfBinsPerDecade : " << param->NumberOfBinsPerDecade() << G4endl; 

  theParams = GetMscParams();
  for( ite = theParams.begin(); ite != theParams.end(); ite++ ) {
    std::map<G4String,double>* Params = (*ite).second;
    for( ite1 = Params->begin(); ite1 != Params->end(); ite1++ ) {
      G4cout << "# EM PARAM # " << (*ite).first << " " << (*ite1).first << " : " << (*ite1).second << G4endl;
    }
  }

  G4cout << "# EM PARAM # Msc RangeFactor : " << param->MscRangeFactor() << G4endl; 
  G4cout << "# EM PARAM # Msc GeomFactor : " << param->MscGeomFactor() << G4endl; 
  G4MscStepLimitType lt = param->MscStepLimitType();
  G4cout << "# EM PARAM # Msc StepLimitType : ";
  if( lt == fMinimal ) {
    G4cout << " Minimal " << G4endl; 
  } else if( lt == fUseSafety ) {
    G4cout << " UseSafety " << G4endl;
  } else if( lt == fUseSafetyPlus ) {
    G4cout << " UseSafetyPlus " << G4endl;
  } else if( lt == fUseDistanceToBoundary ) {
    G4cout << " UseDistanceToBoundary " << G4endl;
  } 
  lt = param->MscMuHadStepLimitType();
  G4cout << "# EM PARAM # Msc MuHadStepLimitType : ";
  if( lt == fMinimal ) {
    G4cout << " Minimal " << G4endl; 
  } else if( lt == fUseSafety ) {
    G4cout << " UseSafety " << G4endl;
  } else if( lt == fUseSafetyPlus ) {
    G4cout << " UseSafetyPlus " << G4endl;
  } else if( lt == fUseDistanceToBoundary ) {
    G4cout << " UseDistanceToBoundary " << G4endl;
  } 
  G4cout << "# EM PARAM # Msc Skin : " << param->MscSkin() << G4endl; 
  G4cout << "# EM PARAM # Msc LateralDisplacement : " << param->LateralDisplacement() << G4endl; 


  G4cout << "# EM PARAM # ELoss LPM : " << param->LPM() << G4endl; 
  //  G4cout << "# EM PARAM # ELoss useAngGeneratorForIonisation : " << param->useAngGeneratorForIonisation() << G4endl; 
  G4cout << "# EM PARAM # ELoss UseCutAsFinalRange : " << param->UseCutAsFinalRange() << G4endl; 
  G4cout << "# EM PARAM # ELoss Spline : " << param->Spline() << G4endl; 
  G4cout << "# EM PARAM # ApplyCuts : " << param->ApplyCuts() << G4endl; 
  G4cout << "# EM PARAM # Fluo " << param->Fluo() << G4endl; 
  G4cout << "# EM PARAM # BeardenFluoDir :" << param->BeardenFluoDir() << G4endl; 
  G4cout << "# EM PARAM # Auger : " << param->Auger() << G4endl; 
  G4cout << "# EM PARAM # AugerCascade : " << param->AugerCascade() << G4endl; 
  G4cout << "# EM PARAM # Pixe : " << param->Pixe() << G4endl; 
  G4cout << "# EM PARAM # DeexcitationIgnoreCut : " << param->DeexcitationIgnoreCut() << G4endl; 

  G4cout << "# EM PARAM # Msc MuHadLateralDisplacement : " << param->MuHadLateralDisplacement() << G4endl; 
  //  G4cout << "# EM PARAM # Msc LatDisplacementBeyondSafety : " << param->LatDisplacementBeyondSafety() << G4endl; 
  G4cout << "# EM PARAM # UseMottCorrection : " << param->UseMottCorrection() << G4endl; // for Ioni
  G4cout << "# EM PARAM # Eloss MinSubRange : " << param->MinSubRange() << G4endl; 
  G4cout << "# EM PARAM # ELoss MaxEnergyForCSDARange : " << param->MaxEnergyForCSDARange() << G4endl; 
  G4cout << "# EM PARAM # ELoss LowestElectronEnergy : " << param->LowestElectronEnergy() << G4endl; 
  G4cout << "# EM PARAM # ELoss LowestMuHadEnergy : " << param->LowestMuHadEnergy() << G4endl; 
  G4cout << "# EM PARAM # ELoss BremsstrahlungTh : " << param->BremsstrahlungTh() << G4endl; 
  G4cout << "# EM PARAM # ELoss LambdaFactor : " << param->LambdaFactor() << G4endl; 
  G4cout << "# EM PARAM # Msc FactorForAngleLimit : " << param->FactorForAngleLimit() << G4endl; 
  G4cout << "# EM PARAM # Msc ThetaLimit : " << param->MscThetaLimit() << G4endl; 
  G4cout << "# EM PARAM # Msc MuHadRangeFactor : " << param->MscMuHadRangeFactor() << G4endl; 
  G4cout << "# EM PARAM # ELoss PIXECrossSectionModel : " << param->PIXECrossSectionModel() << G4endl; 
  G4cout << "# EM PARAM # ELoss PIXEElectronCrossSectionModel : " << param->PIXEElectronCrossSectionModel() << G4endl; 
  /* const std::vector<G4String>& ParticlesPAI() const;
  const std::vector<G4String>& RegionsPAI() const;
  const std::vector<G4String>& TypesPAI() const;
 const std::vector<G4String>& RegionsMicroElec() const;
 const std::vector<G4String>& RegionsDNA() const;
  const std::vector<G4String>& TypesDNA() const;
  */ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::map<G4String, std::map<G4String,double>* > GmPrintEMParamsUA::GetIoniParams() 
{
  std::map<G4String, std::map<G4String,double>* > theIoniParams;
  std::map<G4String,double>* ioniParams;

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if( !pmanager ) continue;
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = procVector->size()-1; ii >= 0; ii-- ) {
      G4VProcess* proc = (*procVector)[ii];
      G4VEnergyLossProcess* procEL = dynamic_cast<G4VEnergyLossProcess*>(proc);
      if( procEL && procEL->IsIonisationProcess() ) {
	G4String partProc = particle->GetParticleName() + "_" + proc->GetProcessName();
	std::map<G4String, std::map<G4String,double>* >::const_iterator ite = theIoniParams.find(partProc);
	if( ite == theIoniParams.end() ) {
	  ioniParams = new std::map<G4String,double>;
	  theIoniParams[partProc] = ioniParams;
	} else {
	  ioniParams = (*ite).second;
	}
	(*ioniParams)["RoverRange"] = procEL->dRoverRange;					
	(*ioniParams)["finalRange"] = procEL->finalRange;					
	(*ioniParams)["subsecID"] = procEL->subsecID;					
	(*ioniParams)["IsIntegral"] = procEL->IsIntegral();					
      }
    }
    
  }

  return theIoniParams;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::map<G4String, std::map<G4String,double>* > GmPrintEMParamsUA::GetMscParams() 
{
  std::map<G4String, std::map<G4String,double>* > theMscParams;
  std::map<G4String,double>* mscParams;

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if( !pmanager ) continue;
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = procVector->size()-1; ii >= 0; ii-- ) {
      G4VProcess* proc = (*procVector)[ii];
      G4VMultipleScattering* procMS = dynamic_cast<G4VMultipleScattering*>(proc);
      if( procMS ) {
	G4String partProc = particle->GetParticleName() + "_" + proc->GetProcessName();
	std::map<G4String, std::map<G4String,double>* >::const_iterator ite = theMscParams.find(partProc);
	if( ite == theMscParams.end() ) {
	  mscParams = new std::map<G4String,double>;
	  theMscParams[partProc] = mscParams;
	} else {
	  mscParams = (*ite).second;
	}
	for( G4int im = 0; im < 3; im++ ) {
	  G4VEmModel* EmModel = procMS->GetModelByIndex(im);
	  //	  G4cout << im << " " << partProc << " EMMODEL " << EmModel << G4endl; //GDEB
	  if( !EmModel ) break;
	  G4String modelName = EmModel->GetName();
	  (*mscParams)[modelName + " MinE"] = EmModel->LowEnergyLimit();
	  (*mscParams)[modelName + " MaxE"] = EmModel->HighEnergyLimit();
	}
      }
    }
    
  }

  return theMscParams;

}

