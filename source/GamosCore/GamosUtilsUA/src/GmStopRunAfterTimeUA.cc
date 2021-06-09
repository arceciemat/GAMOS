#include "GmStopRunAfterTimeUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4RunManager.hh"
#include "G4Timer.hh"

//---------------------------------------------------------------------
GmStopRunAfterTimeUA::GmStopRunAfterTimeUA()
{
 theTimeToStop = GmParameterMgr::GetInstance()->GetNumericValue("GmStopRunAfterTimeUA:Time",DBL_MAX);

 G4cout << UtilsUAVerb(infoVerb) << " GmStopRunAfterTimeUA Time to stop = " << theTimeToStop <<  G4endl;
 
 theTimer = new G4Timer();
 theTimer->Start();
 theCurrentTime = 0.;
}


//---------------------------------------------------------------------
void GmStopRunAfterTimeUA::EndOfEventAction( const G4Event* )
{
  theTimer->Stop();
  //  theCurrentTime += theTimer->GetUserElapsed()/ns;
  G4cout << UtilsUAVerb(debugVerb) << " GmStopRunAfterTimeUA::EndOfEventAction  currentTime= " << theTimer->GetUserElapsed()/CLHEP::ns << " Time to stop= " << theTimeToStop/CLHEP::ns << G4endl;
  if(  theTimer->GetUserElapsed() > theTimeToStop ) {
    G4cout << UtilsUAVerb(warningVerb) << " GmStopRunAfterTimeUA::EndOfEventAction  currentTime= " << theTimer->GetUserElapsed()/CLHEP::ns << " Time to stop= " << theTimeToStop/CLHEP::ns << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
  }

}
