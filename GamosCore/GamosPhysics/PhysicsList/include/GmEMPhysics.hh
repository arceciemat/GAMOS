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
// $Id: GmEMPhysics.hh,v 1.0
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
#ifndef GmEMPhysics_h
#define GmEMPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class GmPhysicsMessenger;

class GmEMPhysics: public G4VModularPhysicsList
{
public:
  GmEMPhysics();
  virtual ~GmEMPhysics();

  virtual void SetCuts();
  virtual G4bool ReplacePhysicsList(const G4String& name);  

  virtual void ConstructParticles();  

  virtual void ConstructProcess();  

protected:
  G4bool electronIsRegistered;
  G4bool positronIsRegistered;
  G4bool gammaIsRegistered;

  GmPhysicsMessenger* messenger;
};

#endif
