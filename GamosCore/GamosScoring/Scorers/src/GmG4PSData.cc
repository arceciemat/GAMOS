// GmG4PSData
#include "GmG4PSData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// (Description)
//   This is a primitive scorer class for scoring from a GmData
//

GmG4PSData::GmG4PSData(G4String name)
  :GmVPrimitiveScorer(name)
{
}

GmG4PSData::~GmG4PSData()
{;}

G4bool GmG4PSData::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  FillScorer( aStep, theData[0]->GetValueFromStep(aStep), aStep->GetPreStepPoint()->GetWeight());

  return TRUE;
}

void GmG4PSData::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSData::DrawAll()
{;}

void GmG4PSData::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  data: " << *(itr->second)
	   << G4endl;
  }
}


void GmG4PSData::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmG4PSData::SetParameters",
		"There should be one parameter: DATA_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }
  
  theData.push_back( Build1Data( params[0] ) );

}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSData::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
