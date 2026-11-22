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
// GmG4PSNofCollision
#include "GmG4PSNofCollision.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring number of collisions inside a
//  Cell.
///////////////////////////////////////////////////////////////////////////////

GmG4PSNofCollision::GmG4PSNofCollision(G4String name) 
  :GmVPrimitiveScorer(name)
{
}

GmG4PSNofCollision::~GmG4PSNofCollision()
{;}

G4bool GmG4PSNofCollision::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  if ( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ) return TRUE;
  if( GmG4Utils::GetSecondariesOfCurrentStep().size() == 0 ) return TRUE;

  FillScorer( aStep, 1.,aStep->GetPreStepPoint()->GetWeight() );

  return TRUE;
}

