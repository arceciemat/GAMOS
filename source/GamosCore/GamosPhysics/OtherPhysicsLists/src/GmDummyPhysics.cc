#include "globals.hh"
#include "GmDummyPhysics.hh"

#include "G4BosonConstructor.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

GmDummyPhysics::GmDummyPhysics(): G4VUserPhysicsList()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 1. * CLHEP::km;

}

GmDummyPhysics::~GmDummyPhysics()
{ 
}

void GmDummyPhysics::ConstructParticle()
{
  G4BosonConstructor bosonC;
  bosonC.ConstructParticle();

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();

}

void GmDummyPhysics::SetCuts()
{  
  // Set the threshold of production equal to the defaultCutValue
  // in the experimental set-up
  G4VUserPhysicsList::SetCutsWithDefault();
    
  if (verboseLevel>0) DumpCutValuesTable();
}

     
void GmDummyPhysics::ConstructProcess()
{
  AddTransportation();

}
                                                                                
