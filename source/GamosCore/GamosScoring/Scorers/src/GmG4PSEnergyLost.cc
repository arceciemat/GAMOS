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
//
// GmG4PSEnergyLost
#include "GmG4PSEnergyLost.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
///////////////////////////////////////////////////////////////////////////////

GmG4PSEnergyLost::GmG4PSEnergyLost(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
}

GmG4PSEnergyLost::~GmG4PSEnergyLost()
{;}

G4bool GmG4PSEnergyLost::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmG4PSEnergyLost::ProcessHits edep " << edep 
	   << " edep " << edep
	   << G4endl;
#endif

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, edep, weight );
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb)) G4cout  << "GmG4PSEnergyLost::ProcessHits  eDepo " << edep*weight << " index " << index << G4endl;
#endif

  return TRUE;
} 


