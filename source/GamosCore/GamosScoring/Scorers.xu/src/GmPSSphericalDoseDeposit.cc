#define _USE_MATH_DEFINES
#include <cmath>

#include "G4VPrimitiveScorer.hh"
#include "GmPSSphericalDoseDeposit.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4EmCalculator.hh"
#include "CLHEP/Random/RandFlat.h"

//--------------------------------------------------------------------
GmPSSphericalDoseDeposit::GmPSSphericalDoseDeposit(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  theGeomUtils = GmGeometryUtils::GetInstance();  
}


//--------------------------------------------------------------------
G4bool GmPSSphericalDoseDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  //----- Choose a random point between pre and post step, in local sphere coordinates
  //  G4double preRad = (aStep->GetPreStepPoint()->GetPosition() - theCentre).mag() ;
  //  G4double postRad = (aStep->GetPostStepPoint()->GetPosition() - theCentre).mag() ;
  G4ThreeVector postPos = aStep->GetPostStepPoint()->GetPosition() - theCentre;
  G4ThreeVector prePos = aStep->GetPreStepPoint()->GetPosition() - theCentre;
  G4ThreeVector pos = prePos + CLHEP::RandFlat::shoot() * (postPos-prePos);
  G4double rad = pos.mag();
  //--- Get the sphere slice 
  G4int index = (rad - theMinRadius ) / theStepRadius;

  if( index < 0 || index >= theNBinsRadius ) {
    return FALSE;
  } else {

    G4double volume;
    if( bTotalVolume ) {
      volume = theTotalVolume;
    } else {
      volume = theVolumes[index];
    }

    G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
    G4double dose  = edep / ( density * volume );
    G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) 
      G4cout << "  GmPSSphericalDoseDeposit::ProcessHits dose " << dose
	     << " edep " << edep
	     << " density " << density / (CLHEP::g/CLHEP::cm3)
	     << " sphere slice " << index
	     << " volume " << volume << G4endl;
#endif
    
    FillScorer( aStep, index, dose, weight );

  }

  return TRUE;
} 

//--------------------------------------------------------------------
void GmPSSphericalDoseDeposit::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 3 
      && params.size() != 6 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSSphericalDoseDeposit::SetParameters",
		"There should be three or six parameters: NSLICES_R MIN_R MAX_R CENTRE_X CENTRE_Y CENTRE_Z",
		FatalErrorInArgument,
		G4String("There are: "+parastr).c_str());
  }
  
  theNBinsRadius = G4int(GmGenUtils::GetValue(params[0]));
  theMinRadius = GmGenUtils::GetValue(params[1]);
  G4double maxRadius = GmGenUtils::GetValue(params[2]);
  theStepRadius = (maxRadius-theMinRadius)/theNBinsRadius;
  if( params.size() == 6 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue(params[3]),GmGenUtils::GetValue(params[4]),GmGenUtils::GetValue(params[5]));
  }
  
  //--- Calculate volumes
  bTotalVolume = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":UseTotalVolume",0));
  if( !bTotalVolume ) {
    for( G4int ii = 0; ii < theNBinsRadius; ii++ ){
      theVolumes[ii] = 4./3.*M_PI*(pow(theMinRadius+(ii+1)*theStepRadius,3)-pow(theMinRadius+ii*theStepRadius,3));
    //    G4cout << " VOLUMES " << ii << " " << theVolumes[ii] << " R " << theMinRadius+ii*theStepRadius << " " <<  4./3.*M_PI << " " << pow(theMinRadius+(ii+1)*theStepRadius,3) << " " << pow(theMinRadius+ii*theStepRadius,3) << G4endl;
    }
  } else {
    theTotalVolume = 4./3.*M_PI*(pow(theMinRadius+theNBinsRadius*theStepRadius,3)-pow(theMinRadius,3));
  }
}

