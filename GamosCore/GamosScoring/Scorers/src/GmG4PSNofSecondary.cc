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
// GmG4PSNofSecondary
#include "GmG4PSNofSecondary.hh"

// (Description)
//   This is a primitive scorer class for scoring number of steps in the
//  Cell.
//


GmG4PSNofSecondary::GmG4PSNofSecondary(G4String name)
  :GmVPrimitiveScorer(name)
{
  fWeighted = 0;
}

GmG4PSNofSecondary::~GmG4PSNofSecondary()
{;}

G4bool GmG4PSNofSecondary::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  //- check for newly produced particle. e.g. first step.
  if ( aStep->GetTrack()->GetCurrentStepNumber() != 1) return FALSE;
  //- check for this is not a primary particle. e.g. ParentID > 0 .
  if ( aStep->GetTrack()->GetParentID() == 0 ) return FALSE;
  //- check the particle if the partifle definition is given.
  //Done with filters  if ( particleDef && particleDef != aStep->GetTrack()->GetDefinition() )
  //    return FALSE;
  //
  //- This is a newly produced secondary particle.
  FillScorerAtPostCheckingRegular( aStep, 1., aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}

void GmG4PSNofSecondary::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSNofSecondary::DrawAll()
{;}

void GmG4PSNofSecondary::PrintAll()
{
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  num of step: " << *(itr->second)
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSNofSecondary::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
