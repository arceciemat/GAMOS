#include "GmDataAncestorStepNumericData.hh"
#include "GamosCore/GamosUserActionMgr/include/GmStepMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataAncestorStepNumericData::GmDataAncestorStepNumericData()
{
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);

  theStepMgr = GmStepMgr::GetInstance();
}

//----------------------------------------------------------------
GmDataAncestorStepNumericData::~GmDataAncestorStepNumericData()
{
}

//----------------------------------------------------------------
G4double GmDataAncestorStepNumericData::GetValueFromStep( const G4Step* aStep, G4int index )
{
  return theAncestorData->GetValueFromStep(theStepMgr->GetAncestorStep(aStep), index );
}

//----------------------------------------------------------------
void GmDataAncestorStepNumericData::SetAncestorData(GmVData* data )
{
  theAncestorData = data;
  G4cout << " GmVDataAncestorStepData::GetStringValueFromStep REG DATA " << data->GetName() << G4endl;
  bInitial = theAncestorData->IsInitial();
  theHMin = theAncestorData->GetHMin();
  theHMax = theAncestorData->GetHMax();
}
