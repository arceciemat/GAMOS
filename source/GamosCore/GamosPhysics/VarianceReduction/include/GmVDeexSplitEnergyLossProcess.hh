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
// $Id: GmVDeexSplitEnergyLossProcess.hh,v 1.5 2018/06/28 15:27:38 arce Exp $
// GEANT4 tag $Name:
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     GmVDeexSplitEnergyLossProcess
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 03.01.2002
//
// Modifications:
//
// 26-12-02 Secondary production moved to derived classes (V.Ivanchenko)
// 20-01-03 Migrade to cut per region (V.Ivanchenko)
// 24-01-03 Make models region aware (V.Ivanchenko)
// 05-02-03 Fix compilation warnings (V.Ivanchenko)
// 13-02-03 SubCutoffProcessors defined for regions (V.Ivanchenko)
// 17-02-03 Fix problem of store/restore tables (V.Ivanchenko)
// 26-02-03 Region dependent step limit (V.Ivanchenko)
// 26-03-03 Add GetDEDXDispersion (V.Ivanchenko)
// 09-04-03 Fix problem of negative range limit for non integral (V.Ivanchenko)
// 13-05-03 Add calculation of precise range (V.Ivanchenko)
// 21-07-03 Add UpdateEmModel method (V.Ivanchenko)
// 12-11-03 G4EnergyLossSTD -> G4EnergyLossProcess (V.Ivanchenko)
// 14-01-04 Activate precise range calculation (V.Ivanchenko)
// 10-03-04 Fix problem of step limit calculation (V.Ivanchenko)
// 30-06-04 make destructor virtual (V.Ivanchenko)
// 05-07-04 fix problem of GenericIons seen at small cuts (V.Ivanchenko)
// 03-08-04 Add DEDX table to all processes for control on integral range(VI)
// 06-08-04 Clear up names of member functions (V.Ivanchenko)
// 27-08-04 Add NeedBuildTables method (V.Ivanchneko)
// 09-09-04 Bug fix for the integral mode with 2 peaks (V.Ivanchneko)
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivanchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivanchenko)
// 11-04-05 Use MaxSecondaryEnergy from a model (V.Ivanchenko)
// 10-01-05 Remove SetStepLimits (V.Ivanchenko)
// 10-01-06 PreciseRange -> CSDARange (V.Ivantchenko)
// 13-01-06 Remove AddSubCutSecondaries and cleanup (V.Ivantchenko)
// 20-01-06 Introduce G4EmTableType and reducing number of methods (VI)
// 26-01-06 Add public method GetCSDARange (V.Ivanchenko)
// 22-03-06 Add SetDynamicMassCharge (V.Ivanchenko)
// 23-03-06 Use isIonisation flag (V.Ivanchenko)
// 13-05-06 Add method to access model by index (V.Ivanchenko)
// 14-01-07 add SetEmModel(index) and SetFluctModel() (mma)
// 15-01-07 Add separate ionisation tables and reorganise get/set methods for
//          dedx tables (V.Ivanchenko)
// 13-03-07 use SafetyHelper instead of navigator (V.Ivanchenko)
// 27-07-07 use stl vector for emModels instead of C-array (V.Ivanchenko)
// 25-09-07 More accurate handling zero xsect in 
//          PostStepGetPhysicalInteractionLength (V.Ivanchenko)
// 27-10-07 Virtual functions moved to source (V.Ivanchenko)
// 15-07-08 Reorder class members for further multi-thread development (VI)
//
// Class Description:
//
// It is the unified energy loss process it calculates the continuous
// energy loss for charged particles using a set of Energy Loss
// models valid for different energy regions. There are a possibility
// to create and access to dE/dx and range tables, or to calculate
// that information on fly.

// -------------------------------------------------------------------
//

#ifndef GmVDeexSplitEnergyLossProcess_h
#define GmVDeexSplitEnergyLossProcess_h 1

#include "G4VEnergyLossProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class GmVDeexSplitEnergyLossProcess : public G4VEnergyLossProcess
{
public:

  GmVDeexSplitEnergyLossProcess(const G4String& name = "EnergyLoss",
		       G4ProcessType type = fElectromagnetic);

  virtual ~GmVDeexSplitEnergyLossProcess();

private:
  //------------------------------------------------------------------------
  // Virtual methods to be implemented in concrete processes
  //------------------------------------------------------------------------

public:
  // AlongStep computations
  G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step&);

protected:
  G4double theNSplit;
  G4double theNSplitInv;
};

#endif
