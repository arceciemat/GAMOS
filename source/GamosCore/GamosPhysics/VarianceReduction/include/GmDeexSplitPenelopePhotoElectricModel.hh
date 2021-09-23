//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GmDeexSplitPenelopePhotoElectricModel.hh,v 1.5 2018/06/28 15:27:37 arce Exp $
// GEANT4 tag $Name:  $
//
// Author: Luciano Pandola
//
// History:
// -----------
// 08 Jan 2010   L. Pandola   1st implementation. 
// 25 May 2011   L. Pandola   Renamed (make v2008 as default Penelope)
// 10 Jun 2011   L. Pandola   Migrated to the new AtomDeexcitation interface
//
// -------------------------------------------------------------------
//
// Class description:
// Low Energy Electromagnetic Physics, Photo-electric effect
// with Penelope Model, version 2008
// -------------------------------------------------------------------

#ifndef GmDeexSplitPenelopePhotoElectricModel_HH
#define GmDeexSplitPenelopePhotoElectricModel_HH 1

#include "GmVPSModelHelper.hh"
#include "G4PenelopePhotoElectricModel.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4AtomicTransitionManager.hh"
#include "G4VAtomDeexcitation.hh"

class G4ParticleDefinition;
class G4DynamicParticle;
class G4MaterialCutsCouple;
class G4Material;

class GmDeexSplitPenelopePhotoElectricModel : public G4PenelopePhotoElectricModel, public GmVPSModelHelper  
{

public:
  
  GmDeexSplitPenelopePhotoElectricModel(const G4ParticleDefinition* p=0,
			       const G4String& processName ="PenPhotoElec");
  
  virtual ~GmDeexSplitPenelopePhotoElectricModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>* fvect,
							   std::vector<G4double>& weightVect,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy,
				   const G4Track& track);


};

#endif

