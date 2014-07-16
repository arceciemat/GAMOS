#include "GmPhysicsXrayRefraction.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "GmXrayRefraction.hh"
#include "G4Gamma.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

// ---------------------------------------------------------------------------
GmPhysicsXrayRefraction::GmPhysicsXrayRefraction()
{ }

// ---------------------------------------------------------------------------
GmPhysicsXrayRefraction::~GmPhysicsXrayRefraction()
{ }

// ---------------------------------------------------------------------------
void GmPhysicsXrayRefraction::ConstructProcess()
{
  // Add GmXrayRefraction for gammas
  
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();

      if (particle == G4Gamma::Gamma()) 
	{
	  G4ProcessManager* pmanager = particle -> GetProcessManager();
	  if( GmG4Utils::CheckProcessExists( pmanager, "XrayRefraction" )) break;
	  pmanager -> AddDiscreteProcess(new GmXrayRefraction);
	}   
    }
}
