#include "GmPhysicsMultipleScattering.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4UrbanMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4MscStepLimitType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmPhysicsMultipleScattering::GmPhysicsMultipleScattering(const G4String& processName, const G4String particleName)
  : G4VMultipleScattering(processName), theParticleName(particleName)
{
  isInitialized = false;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPhysicsMultipleScattering::~GmPhysicsMultipleScattering()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool GmPhysicsMultipleScattering::IsApplicable (const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && !p.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmPhysicsMultipleScattering::InitialiseProcess(const G4ParticleDefinition*)
{
  if(isInitialized) return;

  // initialisation of parameters - defaults for particles other
  // than ions can be overwritten by users
  G4String modelName = "Urban";
  if( theParticleName == "Muon" ) {
    modelName = "Urban";
  } else if( theParticleName == "Hadron" ) {
    modelName = "Urban";
  }
  modelName = GmParameterMgr::GetInstance()->GetStringValue("GmPhysicsMultipleScattering:Model:"+theParticleName,modelName);

  G4VMscModel* mscModel;
  if( modelName == "Urban" ) {
    mscModel = new G4UrbanMscModel();
  }else if( modelName == "WentzelVI" ) {
    mscModel = new G4WentzelVIModel();
  }else if( modelName == "GoudsmitSaunderson" ) {
    mscModel = new G4GoudsmitSaundersonMscModel();
  } else {
    G4Exception("GmPhysicsMultipleScattering::InitialiseProcess",
		"Wrong msc model",
		FatalErrorInArgument,
		G4String("Model is "+modelName+", available models are Urban90, Urban92, Urban93, WentzelVI and GoudsmitSaunderson").c_str());
  }

  AddEmModel(1,mscModel);
  isInitialized = true;
  /*
  G4cout << "GmPhysicsMultipleScattering::InitialiseProcess for " 
	 << p->GetParticleName()
	 << " skin= " << Skin()
	 << " SA= " << steppingAlgorithm
	 << G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmPhysicsMultipleScattering::PrintInfo()
{
  G4cout << "      RangeFactor= " << RangeFactor()
	 << ", step limit type: " << StepLimitType()
         << ", lateralDisplacement: " << LateralDisplasmentFlag()
	 << ", skin= " << Skin()  
	 << ", geomFactor= " << GeomFactor()  
	 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

