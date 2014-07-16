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
// GmG4PSCellCharge
#include "GmG4PSCellCharge.hh"
#include "G4Track.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"


///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell charge.
//   The Cell Charge is defined by  a sum of deposited charge inside the cell.
//
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSCellCharge::GmG4PSCellCharge(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnitName = " [e]";
}

GmG4PSCellCharge::~GmG4PSCellCharge()
{;}

G4bool GmG4PSCellCharge::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSCellCharge::ProcessHits " << G4endl;
#endif
  if( !AcceptByFilter( aStep ) ) return false;

  // Enter or First step of primary.
  if( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary 
      || ( aStep->GetTrack()->GetParentID() == 0 &&
	   aStep->GetTrack()->GetCurrentStepNumber() == 1 ) ){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, charge, aStep->GetPreStepPoint()->GetWeight() );
    }
  }
  
  // Exit
  if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, -charge, aStep->GetPreStepPoint()->GetWeight() );
    } 
  }
  
  return TRUE;
}

void GmG4PSCellCharge::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSCellCharge::DrawAll()
{;}

void GmG4PSCellCharge::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() <<G4endl; 
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  cell charge : " << *(itr->second) << " [e]"
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSCellCharge::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
