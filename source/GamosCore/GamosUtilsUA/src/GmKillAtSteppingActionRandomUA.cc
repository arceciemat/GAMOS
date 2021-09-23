#include "GmKillAtSteppingActionRandomUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"

#include "G4Track.hh"

//---------------------------------------------------------------------------
GmKillAtSteppingActionRandomUA::GmKillAtSteppingActionRandomUA()
{
}

//---------------------------------------------------------------------------
void GmKillAtSteppingActionRandomUA::BeginOfRunAction(const G4Run* )
{
  theProbability = GmParameterMgr::GetInstance()->GetNumericValue(theName+":Probability",1.);

}

//---------------------------------------------------------------------------
void GmKillAtSteppingActionRandomUA::UserSteppingAction( const G4Step* aStep )
{
  if( CLHEP::RandFlat::shoot() > theProbability ) return;
  
  //  G4cout << "  GmKillAtSteppingActionUA::UserSteppingAction(  " << G4endl;
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  aTrack->SetTrackStatus(fStopAndKill);

}
