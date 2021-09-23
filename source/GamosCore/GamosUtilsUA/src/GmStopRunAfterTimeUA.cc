#include "GmStopRunAfterTimeUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4RunManager.hh"
#include "G4Timer.hh"

//---------------------------------------------------------------------
GmStopRunAfterTimeUA::GmStopRunAfterTimeUA()
{
 theTimeToStop = GmParameterMgr::GetInstance()->GetNumericValue("GmStopRunAfterTimeUA:Time",DBL_MAX);

#ifndef GAMOS_NO_VERBOSE
  if(UtilsUAVerb(infoVerb) ) G4cout << " GmStopRunAfterTimeUA Time to stop = " << theTimeToStop <<  G4endl;
#endif
 theTimer = new G4Timer();
 theTimer->Start();
 theCurrentTime = 0.;
}


//---------------------------------------------------------------------
void GmStopRunAfterTimeUA::EndOfEventAction( const G4Event* )
{
  theTimer->Stop();
  //  theCurrentTime += theTimer->GetUserElapsed()/ns;

#ifndef GAMOS_NO_VERBOSE
  if (UtilsUAVerb(debugVerb)) G4cout << " GmStopRunAfterTimeUA::EndOfEventAction  currentTime= " << theTimer->GetUserElapsed()/CLHEP::ns << " Time to stop= " << theTimeToStop/CLHEP::ns << G4endl;
#endif
  if(  theTimer->GetUserElapsed() > theTimeToStop ) {

#ifndef GAMOS_NO_VERBOSE
	  if (UtilsUAVerb(warningVerb)) G4cout << " GmStopRunAfterTimeUA::EndOfEventAction  currentTime= " << theTimer->GetUserElapsed()/CLHEP::ns << " Time to stop= " << theTimeToStop/CLHEP::ns << G4endl;
#endif
	  G4RunManager::GetRunManager()->AbortRun();
  }

}
