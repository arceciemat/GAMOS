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
// GmG4PSMinKinEAtGeneration

#include "GmG4PSMinKinEAtGeneration.hh"
#include "G4UnitsTable.hh"

// (Description)
//   This is a primitive scorer class for scoring minimum energy of
//  newly produced particles in the geometry.

GmG4PSMinKinEAtGeneration::GmG4PSMinKinEAtGeneration(G4String name)
:GmVPrimitiveScorer(name)
{
  fWeighted = FALSE;
  bScoreErrors = FALSE;
  theNEventsType = SNET_ByRun;

  theUnit = 1.;
  theUnitName = G4String("MeV");

}

GmG4PSMinKinEAtGeneration::~GmG4PSMinKinEAtGeneration()
{;}

G4bool GmG4PSMinKinEAtGeneration::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  // ===============
  //  First Step, 
  //  Confirm this is a newly produced secondary.
  //  
  //- check for newly produced particle. e.g. Step number is 1.
  if ( aStep->GetTrack()->GetCurrentStepNumber() != 1) return FALSE;
  //- check for this is not a primary particle. e.g. ParentID != 0 .
  if ( aStep->GetTrack()->GetParentID() == 0 ) return FALSE;

  //===============================================
  //- This is a newly produced secondary particle.
  //===============================================

  // ===============
  //  Second Step,
  //  Confirm this track has lower energy than previous one.
  //

  // -Kinetic energy of this particle at the starting point.
  G4double kinetic = aStep->GetPreStepPoint()->GetKineticEnergy();

  // -Stored value in the current HitsMap.
  G4int  index = GetIndex(aStep);
  G4double*   mapValue=  ((*EvtMap)[index]);

  // -
  // If mapValue exits (e.g not NULL ), compare it with 
  // current track's kinetic energy.
  if ( mapValue && ( kinetic > *mapValue ) ) return FALSE;

  // -
  // Current Track is a newly produced secondary and has lower
  // kinetic energy than previous one in this geometry.
  //
  EvtMap->set(index, kinetic);
  return TRUE;
}

