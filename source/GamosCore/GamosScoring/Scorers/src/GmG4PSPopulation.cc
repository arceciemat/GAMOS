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
// GmG4PSPopulation
#include "GmG4PSPopulation.hh"


///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring population in a cell.
//
///////////////////////////////////////////////////////////////////////////////

GmG4PSPopulation::GmG4PSPopulation(G4String name)
  :GmVPrimitiveScorer(name)
{
  fWeighted = 0;
}

GmG4PSPopulation::~GmG4PSPopulation()
{;}


#include "CLHEP/Random/RandFlat.h"
G4bool GmG4PSPopulation::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4int index = GetIndex(aStep);
  G4TrackLogger& tlog = fCellTrackLogger[index];
  if (tlog.FirstEnterance(aStep->GetTrack()->GetTrackID())) {
    /*      G4int nco = int( CLHEP::RandFlat::shoot() * 2);
    G4cout << " GmG4PSPopulation " << nco << G4endl;
    FillScorer( aStep, nco, aStep->GetPreStepPoint()->GetWeight());*/
    FillScorer( aStep, 1., aStep->GetPreStepPoint()->GetWeight());
  }


  return TRUE;
}

void GmG4PSPopulation::EndOfEvent(G4HCofThisEvent*)
{
  fCellTrackLogger.clear();
}

void GmG4PSPopulation::clear(){
  EvtMap->clear();
  fCellTrackLogger.clear();
}

void GmG4PSPopulation::DrawAll()
{;}

void GmG4PSPopulation::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  population: " << *(itr->second)
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSPopulation::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
