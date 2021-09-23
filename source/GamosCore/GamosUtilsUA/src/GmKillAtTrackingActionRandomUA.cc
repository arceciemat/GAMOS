#include "GmKillAtTrackingActionRandomUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"

#include "G4Track.hh"

//---------------------------------------------------------------------------
GmKillAtTrackingActionRandomUA::GmKillAtTrackingActionRandomUA()
{
}

//---------------------------------------------------------------------------
void GmKillAtTrackingActionRandomUA::BeginOfRunAction(const G4Run* )
{
  theProbability = GmParameterMgr::GetInstance()->GetNumericValue(theName+":Probability",1.);

}
//---------------------------------------------------------------------------
void GmKillAtTrackingActionRandomUA::PreUserTrackingAction(const G4Track* aTrack)
{
  if( CLHEP::RandFlat::shoot() > theProbability ) return;
  
  G4Track* aTrackNC = const_cast<G4Track*>(aTrack);
  aTrackNC->SetTrackStatus(fStopAndKill);

}
