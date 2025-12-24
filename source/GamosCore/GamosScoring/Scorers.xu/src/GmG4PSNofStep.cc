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
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

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
      !GmGeometryUtils::GetInstance()->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) ) {
    FillScorer( aStep, 1., aStep->GetPreStepPoint()->GetWeight() );
  } else { // use the last voxel traversed
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      FillScorer( aStep, (*ite).first, 1., aStep->GetPreStepPoint()->GetWeight() );
    }
  }

  return TRUE;
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
		JustWarning,
		G4String("They are: "+parastr).c_str());
  } else {
    boundaryFlag = G4bool( GmGenUtils::GetInteger( params[0] ) );
  }
}
