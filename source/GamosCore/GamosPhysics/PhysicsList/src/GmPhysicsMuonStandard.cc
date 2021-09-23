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
// $Id: GmPhysicsMuonStandard.cc; May 2005
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

#include "GmPhysicsMuonStandard.hh"
#include "G4ProcessManager.hh"

//muon:
#include "G4hIonisation.hh"
#include "GmPhysicsMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCapture.hh" 
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

GmPhysicsMuonStandard::GmPhysicsMuonStandard(const G4String& name, G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsMuonStandard::~GmPhysicsMuonStandard()
{ }

void GmPhysicsMuonStandard::ConstructParticle()
{
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
}

void GmPhysicsMuonStandard::ConstructProcess()
{
  // Add standard processes for muons
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
     
      if(( particleName == "mu+")|| (particleName == "mu-" ))
	{
           //muon
	  G4VProcess* aMultipleScattering = new GmPhysicsMultipleScattering("msc","Muon");
	  G4VProcess* aBremsstrahlung     = new G4MuBremsstrahlung();
	  G4VProcess* aPairProduction     = new G4MuPairProduction();
	  G4VProcess* anIonisation        = new G4MuIonisation();
	  //
	  // add processes
	  pmanager -> AddProcess(aMultipleScattering, -1, 1, 1);
	  pmanager -> AddProcess(anIonisation, -1, 2, 2);
	  pmanager -> AddProcess(aBremsstrahlung, -1, 3, 3);
	  pmanager -> AddProcess(aPairProduction,-1, 4, 4);
	  //
	  if( particleName == "mu-" )
	    pmanager -> AddProcess(new G4MuonMinusCapture(), 0,-1,-1);
	}
    }
  G4cout << "muon processed loaded"<<G4endl;
}
