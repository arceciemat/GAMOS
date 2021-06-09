#include "GmG4PSTrackLength.hh"

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
}


GmG4PSTrackLength::~GmG4PSTrackLength()
{;}

G4bool GmG4PSTrackLength::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;
  
  G4double trklength  = aStep->GetStepLength();
  if ( trklength == 0. ) return FALSE;
  
  if( !IsRegularScoring(aStep) ) {
    if(multiplyKinE) trklength *= aStep->GetPreStepPoint()->GetKineticEnergy();
    if(divideByVelocity) trklength /= aStep->GetPreStepPoint()->GetVelocity();
    
    FillScorer( aStep, trklength, aStep->GetPreStepPoint()->GetWeight());
    
  } else {
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      G4double trklength  = (*ite).second;
      if(multiplyKinE) trklength *= aStep->GetPreStepPoint()->GetKineticEnergy();
      if(divideByVelocity) trklength /= aStep->GetPreStepPoint()->GetVelocity();
      
      FillScorer( aStep, trklength, aStep->GetPreStepPoint()->GetWeight());
    }
  }
  
  return TRUE;
  
}

void GmG4PSTrackLength::EndOfEvent(G4HCofThisEvent*)
{;}


void GmG4PSTrackLength::DrawAll()
{;}

void GmG4PSTrackLength::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first << "  track length: " ;
    if ( multiplyKinE && !divideByVelocity ){
      G4cout << *(itr->second)/(CLHEP::mm*CLHEP::MeV) <<" CLHEP::mm*CLHEP::MeV";
    } else if ( !multiplyKinE && divideByVelocity ){
      G4cout << *(itr->second)*CLHEP::second <<" /second";
    } else if ( multiplyKinE && divideByVelocity) {
      G4cout << *(itr->second)/CLHEP::MeV*CLHEP::second <<" CLHEP::MeV/second";
    } else {
      G4cout  << G4BestUnit(*(itr->second),"Length");
    }
    G4cout << G4endl;
  }
}

void GmG4PSTrackLength::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSWithDirection::SetParameters",
		"There should be two parameters: multiplyKinE divideByVelocity ",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }
  
  multiplyKinE = GmGenUtils::GetBoolean( params[0] );
  divideByVelocity = GmGenUtils::GetBoolean( params[1] );
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

 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSTrackLength::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
