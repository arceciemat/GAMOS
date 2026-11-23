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
// GmG4PSPassageTrackLength
#include "GmG4PSPassageTrackLength.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only track length.
//   The tracks which passed a geometry is taken into account.
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSPassageTrackLength::GmG4PSPassageTrackLength(G4String name)
  :GmG4PSTrackLength(name),fCurrentTrkID(-1),fTrackLength(0.)
{
  fWeighted = 0;
  theUnit = 1.;
  theUnitName = G4String("mm");

}

GmG4PSPassageTrackLength::~GmG4PSPassageTrackLength()
{;}

G4bool GmG4PSPassageTrackLength::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  if ( IsPassed(aStep) ) {
    FillScorer( aStep, fTrackLength, aStep->GetPreStepPoint()->GetWeight());
  }

  return TRUE;
}

G4bool GmG4PSPassageTrackLength::IsPassed(G4Step* aStep){
  G4bool Passed = FALSE;

  G4bool IsEnter = aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary;
  G4bool IsExit  = aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary;

  G4int  trkid  = aStep->GetTrack()->GetTrackID();
  G4double trklength  = aStep->GetStepLength();

  if ( IsEnter &&IsExit ){         // Passed at one step
    fTrackLength = trklength;      // Track length is absolutely given.
    Passed = TRUE;                 
  }else if ( IsEnter ){            // Enter a new geometry
    fCurrentTrkID = trkid;         // Resetting the current track.
    fTrackLength  = trklength;     
  }else if ( IsExit ){             // Exit a current geometry
    if ( fCurrentTrkID == trkid ) {
      fTrackLength  += trklength;  // Adding the track length to current one,
      Passed = TRUE;               // if the track is same as entered.
    }
  }else{                           // Inside geometry
    if ( fCurrentTrkID == trkid ){ // Adding the track length to current one ,
      fTrackLength  += trklength;  // if the track is same as entered.
    }
  }

  return Passed;
}

