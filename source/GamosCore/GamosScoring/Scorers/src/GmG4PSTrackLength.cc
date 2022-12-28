#include "GmG4PSTrackLength.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4UnitsTable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring sum of track length.
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSTrackLength::GmG4PSTrackLength(G4String name)
  :GmVPrimitiveScorer(name),multiplyKinE(false),divideByVelocity(false)
{
  theUnitName = "mm";
}


GmG4PSTrackLength::~GmG4PSTrackLength()
{;}

G4bool GmG4PSTrackLength::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;
  
  G4double trklength  = aStep->GetStepLength();
  if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSTrackLength::ProcessHits trklength " <<trklength << " " <<multiplyKinE << " " << divideByVelocity << G4endl;
  if ( trklength == 0. ) return FALSE;
  
  if( !IsRegularScoring(aStep) ) {
    if(multiplyKinE) trklength *= aStep->GetPreStepPoint()->GetKineticEnergy();
    if(divideByVelocity) trklength /= aStep->GetPreStepPoint()->GetVelocity();
    
    FillScorer( aStep, trklength, aStep->GetPreStepPoint()->GetWeight());
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSTrackLength::ProcessHits trklength " <<trklength << " wei " << aStep->GetPreStepPoint()->GetWeight() << G4endl;
#endif
      
  } else {
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      G4double trklength2  = (*ite).second;
      if(multiplyKinE) trklength2 *= aStep->GetPreStepPoint()->GetKineticEnergy();
      if(divideByVelocity) trklength2 /= aStep->GetPreStepPoint()->GetVelocity();
      
      FillScorer( aStep, trklength2, aStep->GetPreStepPoint()->GetWeight());
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSTrackLength::ProcessHits trklength2 " <<trklength2 << " wei " << aStep->GetPreStepPoint()->GetWeight() << G4endl;
#endif
    }
  }
  
  return TRUE;
  
}

void GmG4PSTrackLength::SetParameters( const std::vector<G4String>& params)
{
  /*  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
       G4Exception("GmG4PSTrackLength::SetParameters",
		"There should be two parameters: multiplyKinE divideByVelocity ",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
    
		} */

  if( params.size() >= 2 ){
    divideByVelocity = GmGenUtils::GetBoolean( params[1] );
    multiplyKinE = GmGenUtils::GetBoolean( params[0] );
  } else if( params.size() >= 1 ){
    divideByVelocity = 0;
    multiplyKinE = GmGenUtils::GetBoolean( params[0] );
  } else {
    divideByVelocity = 0;
    multiplyKinE = 0;
  }
}


G4double GmG4PSTrackLength::GetUnit() const
{
  if ( multiplyKinE && !divideByVelocity ){
    return CLHEP::mm*CLHEP::MeV; //  /(CLHEP::mm*CLHEP::MeV)
  } else if ( !multiplyKinE && divideByVelocity ){
    return CLHEP::second; // second;
  } else if ( multiplyKinE && divideByVelocity) {
    return CLHEP::second/CLHEP::MeV; // second/CLHEP::MeV;
  } else {
    return CLHEP::mm; // /CLHEP::mm
  }
}

G4String GmG4PSTrackLength::GetUnitName() const
{
  if ( multiplyKinE && !divideByVelocity ){
    return " CLHEP::mm*CLHEP::MeV";
  } else if ( !multiplyKinE && divideByVelocity ){
    return " /second";
  } else if ( multiplyKinE && divideByVelocity) {
    return " CLHEP::MeV/second";
  } else {
    return "CLHEP::mm";
  }
}

