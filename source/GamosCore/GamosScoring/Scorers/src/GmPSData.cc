#include "GmPSData.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UnitsTable.hh"

GmPSData::GmPSData(G4String name)
  :GmVPrimitiveScorer(name)
{
}


GmPSData::~GmPSData()
{;}

G4bool GmPSData::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double val = theData->GetValueFromStep(aStep);
  
  FillScorer( aStep, val, aStep->GetPreStepPoint()->GetWeight());
  
  return TRUE;
  
}


void GmPSData::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSData::SetParameters",
		"There should be one parameter: DATA_NAME ",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  theData = GmDataMgr::GetInstance()->BuildData( params[0] );
}


G4double GmPSData::GetUnit() const
{
  return 1.;
}

G4String GmPSData::GetUnitName() const
{
  return theData->GetName();
}

