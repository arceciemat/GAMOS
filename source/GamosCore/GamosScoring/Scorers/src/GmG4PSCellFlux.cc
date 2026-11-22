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
// GmG4PSCellFlux
#include "GmG4PSCellFlux.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell flux.
//   The Cell Flux is defined by  a sum of track length divided 
//   by the geometry volume, where all of the tracks in the geometry 
//   are taken into account. 
//
//   If you want to score only tracks passing through the geometry volume,
//  please use G4PSPassageCellFlux.
//
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSCellFlux::GmG4PSCellFlux(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.E-2;
  theUnitName = "cm-2";

  bUseVolume = true;
}

GmG4PSCellFlux::~GmG4PSCellFlux()
{;}

G4bool GmG4PSCellFlux::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double stepLength = aStep->GetStepLength();
  if ( stepLength == 0. ) return FALSE; 
  G4double volume = GetVolume(aStep);
  G4double CellFlux = stepLength / volume ;
  FillScorer( aStep, CellFlux, aStep->GetPreStepPoint()->GetWeight() );
  
  return TRUE;
}

