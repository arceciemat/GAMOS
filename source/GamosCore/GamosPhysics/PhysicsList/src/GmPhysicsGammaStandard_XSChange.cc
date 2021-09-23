//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
// $Id: GmPhysicsGammaStandard_XSChange.cc; May 2005
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the National Institute for Nuclear Physics, Catania, Italy
// (b) National Institute for Nuclear Physics Section of Genova, genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------
#include "GmPhysicsGammaStandard_XSChange.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GmComptonScattering_XSChange.hh"
#include "GmGammaConversion_XSChange.hh"
#include "GmPhotoElectricEffect_XSChange.hh"
#include "GmRayleighScattering_XSChange.hh"
#include "G4RayleighScattering.hh" //GDEB

GmPhysicsGammaStandard_XSChange::GmPhysicsGammaStandard_XSChange(const G4String& name, G4int type): GmVPhysicsGamma(name,type)
{ }

GmPhysicsGammaStandard_XSChange::~GmPhysicsGammaStandard_XSChange()
{ }

void GmPhysicsGammaStandard_XSChange::ConstructProcess()
{
  // Add standard processes for gammas
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();

      if (particleName == "gamma") 
	{
	  manager -> AddDiscreteProcess(new GmPhotoElectricEffect_XSChange);
	  manager -> AddDiscreteProcess(new GmComptonScattering_XSChange);
	  manager -> AddDiscreteProcess(new GmGammaConversion_XSChange); 
	  //	  manager -> AddDiscreteProcess(new G4RayleighScattering);
	  manager -> AddDiscreteProcess(new GmRayleighScattering_XSChange);
	}   
    }
}
