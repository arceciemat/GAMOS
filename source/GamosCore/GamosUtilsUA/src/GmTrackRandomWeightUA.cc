#include "GmTrackRandomWeightUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"

//------------------------------------------------------------------
GmTrackRandomWeightUA::GmTrackRandomWeightUA()
{
}

//------------------------------------------------------------------
GmTrackRandomWeightUA::~GmTrackRandomWeightUA()
{
}

//------------------------------------------------------------------
void GmTrackRandomWeightUA::BeginOfRunAction( const G4Run* )
{
  theMinWeight = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MinWeight",0.);
  theMaxWeight = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaxWeight",1.);
  theWeightSpan = theMaxWeight - theMinWeight;

}

//------------------------------------------------------------------
void GmTrackRandomWeightUA::PreUserTrackingAction( const G4Track* aTrack )
{
  G4double wei = theMinWeight + CLHEP::RandFlat::shoot()*theWeightSpan;
  G4Track* trackNC = const_cast<G4Track*>(aTrack);
  trackNC->SetWeight(wei);
}

