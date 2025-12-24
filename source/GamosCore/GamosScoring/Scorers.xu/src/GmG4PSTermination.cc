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
// GmG4PSTermination
#include "GmG4PSTermination.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only terminated tracks inside
//  the cell.
//
///////////////////////////////////////////////////////////////////////////////

GmG4PSTermination::GmG4PSTermination(G4String name)
  :GmVPrimitiveScorer(name)
{
  fWeighted = 0;
}

GmG4PSTermination::~GmG4PSTermination()
{;}

G4bool GmG4PSTermination::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  if ( aStep->GetTrack()->GetTrackStatus() != fStopAndKill ) return FALSE;

  FillScorer( aStep, 1.0, aStep->GetPreStepPoint()->GetWeight());

  return TRUE;
}

