#include "GmStepRandomWeightUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"

//------------------------------------------------------------------
GmStepRandomWeightUA::GmStepRandomWeightUA()
{
}

//------------------------------------------------------------------
GmStepRandomWeightUA::~GmStepRandomWeightUA()
{
}

//------------------------------------------------------------------
void GmStepRandomWeightUA::BeginOfRunAction( const G4Run* )
{
  theMinWeight = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MinWeight",0.);
  theMaxWeight = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaxWeight",1.);
  theWeightSpan = theMaxWeight - theMinWeight;

}

//------------------------------------------------------------------
void GmStepRandomWeightUA::UserSteppingAction( const G4Step* aStep )
{
  G4double wei = theMinWeight + CLHEP::RandFlat::shoot()*theWeightSpan;
  aStep->GetTrack()->SetWeight(wei);
}

