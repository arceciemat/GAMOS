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
// $Id: GmDeexSplitLivermorePhotoElectricModel.hh,v 1.5 2018/06/28 15:27:37 arce Exp $
// GEANT4 tag $Name:  $
//
// Author: Sebastien Incerti
//         30 October 2008
//         on base of G4LowEnergyPhotoElectric developed by A.Forti and M.G.Pia
//
// 15 Mar 2010   L. Pandola, removed methods to set explicitely fluorescence cuts.
//               Main cuts from G4ProductionCutsTable are always used
// 30 May 2011   A Mantero & V Ivanchenko Migration to model design for deexcitation
//

#ifndef GmDeexSplitLivermorePhotoElectricModel_h
#define GmDeexSplitLivermorePhotoElectricModel_h 1

#include "G4LivermorePhotoElectricModel.hh"
#include "GmVPSModelHelper.hh"

class G4ParticleChangeForGamma;
class G4VCrossSectionHandler;
class G4VAtomDeexcitation;
class G4VPhotoElectricAngularDistribution;
class G4AtomicTransitionManager;

using namespace CLHEP;

class GmDeexSplitLivermorePhotoElectricModel : public G4LivermorePhotoElectricModel, public GmVPSModelHelper
{

public:

  GmDeexSplitLivermorePhotoElectricModel(const G4ParticleDefinition* p = 0, 
				const G4String& nam = "LivermorePhElectric");

  virtual ~GmDeexSplitLivermorePhotoElectricModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>* fvect,
				   std::vector<G4double>& weightVect,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy,
				   const G4Track& track);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
