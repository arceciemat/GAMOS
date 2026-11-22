#include "GmDataAncestorStepStringData.hh"
#include "GamosCore/GamosUserActionMgr/include/GmStepMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataAncestorStepStringData::GmDataAncestorStepStringData()
{
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);

  theStepMgr = GmStepMgr::GetInstance();
}

//----------------------------------------------------------------
GmDataAncestorStepStringData::~GmDataAncestorStepStringData()
{
}

//----------------------------------------------------------------
G4String GmDataAncestorStepStringData::GetStringValueFromStep( const G4Step* aStep)
{
  return theAncestorData->GetStringValueFromStep(theStepMgr->GetAncestorStep(aStep) );
}

//----------------------------------------------------------------
void GmDataAncestorStepStringData::SetAncestorData(GmVData* data )
{
  theAncestorData = data;
  G4cout << " GmVDataAncestorStepData::GetStringValueFromStep REG DATA " << data->GetName() << G4endl;
  bInitial = theAncestorData->IsInitial();
  theHMin = theAncestorData->GetHMin();
  theHMax = theAncestorData->GetHMax();
}
