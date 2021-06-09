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
// GmG4PSNofStep
#include "GmG4PSNofStep.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// (Description)
//   This is a primitive scorer class for scoring number of steps in the
//  Cell.
//

GmG4PSNofStep::GmG4PSNofStep(G4String name)
  :GmVPrimitiveScorer(name),boundaryFlag(false)
{
  theUnit = 1.;
  fWeighted = 0;
}

GmG4PSNofStep::~GmG4PSNofStep()
{;}

G4bool GmG4PSNofStep::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  if ( boundaryFlag ) {
      if ( aStep->GetStepLength() == 0. ) return FALSE;
  }

  if( bUseClassifierIndex ||
      !bSkipEqualMaterials ||
      G4RegularNavigationHelper::Instance()->theStepLengths.size() <= 1 ||
      !GmRegularParamUtils::GetInstance()->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) ) {
    FillScorer( aStep, 1., aStep->GetPreStepPoint()->GetWeight() );
  } else { // use the last voxel traversed
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      FillScorer( aStep, (*ite).first, 1., aStep->GetPreStepPoint()->GetWeight() );
    }
  }

  return TRUE;
}

void GmG4PSNofStep::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSNofStep::DrawAll()
{;}

void GmG4PSNofStep::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  num of step: " << *(itr->second)
	   << G4endl;
  }
}


void GmG4PSNofStep::SetParameters( const std::vector<G4String>& params)
{
 if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmG4PSNofStep::SetParameters",
		"There should be one parameter: boundaryFlag",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 boundaryFlag = G4bool( GmGenUtils::GetInteger( params[0] ) );

}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSNofStep::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
