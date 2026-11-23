#include "GmKillAtStackingActionUA.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------------
GmKillAtStackingActionUA::GmKillAtStackingActionUA()
{

}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmKillAtStackingActionUA::ClassifyNewTrack(const G4Track* )
{
  return fKill;
}
