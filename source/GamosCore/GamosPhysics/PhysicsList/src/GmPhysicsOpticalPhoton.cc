#include "GmPhysicsOpticalPhoton.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4Scintillation.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsOpticalPhoton::GmPhysicsOpticalPhoton()
{ }

GmPhysicsOpticalPhoton::~GmPhysicsOpticalPhoton()
{ }

void GmPhysicsOpticalPhoton::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhoton();
}

void GmPhysicsOpticalPhoton::ConstructProcess()
{
  // Add processes for optical gammas
  
  G4Scintillation* theScintillationProcess = new G4Scintillation();
  G4double yieldFactor = GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsOpticalPhoton:YieldFactor",1.);
  theScintillationProcess->SetScintillationYieldFactor(yieldFactor);
  theScintillationProcess->SetFiniteRiseTime(true);

  G4bool bTrackSeco1st = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsOpticalPhoton:TrackSecondariesFirst",1));
  theScintillationProcess->SetTrackSecondariesFirst(bTrackSeco1st);
  
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4bool bFound = false;

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
      if (theScintillationProcess->IsApplicable(*particle)) 
	{
	  if( GmG4Utils::CheckProcessExists( pmanager, "Scintillation" )) break;
	  pmanager->AddProcess(theScintillationProcess);
	  pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
	}     
      else if (particleName == "opticalphoton") 
	{
	  bFound = true;
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpAbsorption" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpAbsorption);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpRayleigh" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpRayleigh);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpBoundary" )) break;
	  G4OpBoundaryProcess* theBoundaryProcess  = new G4OpBoundaryProcess();
	  pmanager -> AddDiscreteProcess(theBoundaryProcess);
	}   
    }

  if( !bFound ) {
    G4Exception("GmPhysicsOpticalPhoton::ConstructProcess",
		"Fatal error",
		FatalErrorInArgument,
		"opticalphoton processes cannot be added if opticalphoton particle is not created, please add the command '/gamos/phsysics/createParticle opticalphoton' before '/run/initialize'");
  }

}
