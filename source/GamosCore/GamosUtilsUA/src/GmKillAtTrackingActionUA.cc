#include "GmKillAtTrackingActionUA.hh"

#include "G4UserTrackingAction.hh"

#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"

#include "GmUtilsUAVerbosity.hh"

#include "G4TrackingManager.hh"
#include "G4EventManager.hh"

//---------------------------------------------------------------------------
GmKillAtTrackingActionUA::GmKillAtTrackingActionUA()
{

}

//----------------------------------------------------------------
void GmKillAtTrackingActionUA::PreUserTrackingAction(const G4Track* aTrack)
{
  G4Track* aTrackNC = const_cast<G4Track*>(aTrack);
  aTrackNC->SetTrackStatus(fStopAndKill);

}
 
