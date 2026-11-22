#include "GmKillAllUA.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------------
GmKillAllUA::GmKillAllUA()
{

}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmKillAllUA::ClassifyNewTrack(const G4Track* )
{
  return fKill;
}

//---------------------------------------------------------------------------
void GmKillAllUA::UserSteppingAction( const G4Step* aStep )
{
  //  G4cout << "  GmKillAllUA::UserSteppingAction(  " << G4endl;
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  aTrack->SetTrackStatus(fStopAndKill);

}
