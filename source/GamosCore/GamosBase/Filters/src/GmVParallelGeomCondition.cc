#include "GmVParallelGeomCondition.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4ParticleTable.hh"
#include "G4VTouchable.hh"
#include "G4StepPoint.hh"

//-------------------------------------------------------------------------
void GmVParallelGeomCondition::FillParallelWorldProcesses()
{
  GmGetParticleMgr* particleMgr = GmGetParticleMgr::GetInstance();
  std::vector<G4ParticleDefinition*> particles = particleMgr->GetG4ParticleList("*");

  for( size_t ip = 0; ip < particles.size(); ip++ ){
    G4ParticleDefinition* particle = particles[ip];
    std::vector<G4VProcess*> processes = particleMgr->GetG4ProcessList(particle->GetParticleName(),"ParaWorldProc*",1);
    /*    if( processes.size() == 0 ) {
      G4Exception("GmVParallelGeomCondition::FillParallelWorldProcesses",
		  "No G4ParallelWorldScoringProcess named 'ParaWorldProc*' found",
		  FatalErrorInArgument,
		  "Check that you have not defined the filter before '/run/initialize'");
		  } */
    std::vector<G4ParallelWorldScoringProcess*> pwsProcesses;
    for( size_t ii = 0; ii < processes.size(); ii++ ) {    
      pwsProcesses.push_back((G4ParallelWorldScoringProcess*)(processes[ii]));
    }
    theParallelWorldProcesses[particle] = pwsProcesses; 
  }

}

//-------------------------------------------------------------------------
std::vector<G4ParallelWorldScoringProcess*> GmVParallelGeomCondition::GetParticleParallelWorldProcesses(G4ParticleDefinition* particle )
{
  std::map< G4ParticleDefinition*, std::vector<G4ParallelWorldScoringProcess*> >::const_iterator ite = theParallelWorldProcesses.find(particle);
  if( ite == theParallelWorldProcesses.end() ) {
    G4Exception("GmVParallelGeomConditionGetParticleParallelWorldProcesses",
		"",
		FatalException,
		("No G4ParallelWorldScoringProcess found for particle " + particle->GetParticleName()).c_str());
  }

  return (*ite).second;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmVParallelGeomCondition::GetTouchableFromStepPoint(const G4StepPoint* sp)
{
  return sp->GetTouchable();
}
