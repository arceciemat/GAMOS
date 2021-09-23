#include "GmKillAtStackingActionRandomUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"

#include "G4Track.hh"
#include "G4RunManager.hh"

//---------------------------------------------------------------------------
GmKillAtStackingActionRandomUA::GmKillAtStackingActionRandomUA() :
  bKillEvent(false)
{
}


//---------------------------------------------------------------------------
void GmKillAtStackingActionRandomUA::BeginOfRunAction(const G4Run* )
{
  theProbability = GmParameterMgr::GetInstance()->GetNumericValue(theName+":Probability",1.);
  bKillEvent = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":KillEvent",0));
  std::cout << "bKillEvent: " << bKillEvent << " P: " << theProbability << std::endl;
}

void
GmKillAtStackingActionRandomUA::BeginOfEventAction(const G4Event* )
{
  if( !bKillEvent ) return ;
  
  isEventKilled = false;
  if ( CLHEP::RandFlat::shoot() < theProbability ) 
  {
    isEventKilled = true;
    //	G4RunManager::GetRunManager()->AbortEvent();
  }
}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmKillAtStackingActionRandomUA::ClassifyNewTrack(const G4Track* )
{
  if( bKillEvent ) 
  {
  	if ( !isEventKilled ) 
  	{
      return fUrgent;
  	}
  	return fKill;
  }
    
  if( CLHEP::RandFlat::shoot() > theProbability ) return fUrgent;
  return fKill;
}
