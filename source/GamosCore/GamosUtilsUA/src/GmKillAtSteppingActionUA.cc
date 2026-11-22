#include "GmKillAtSteppingActionUA.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------------
GmKillAtSteppingActionUA::GmKillAtSteppingActionUA()
{

}

//---------------------------------------------------------------------------
void GmKillAtSteppingActionUA::UserSteppingAction( const G4Step* aStep )
{
  //  G4cout << "  GmKillAtSteppingActionUA::UserSteppingAction(  " << G4endl;
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  aTrack->SetTrackStatus(fStopAndKill);

}
