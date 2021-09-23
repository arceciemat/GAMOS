#include "GamosCore/GamosSD/include/GmVDeadTimeDUList.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "globals.hh"


//----------------------------------------------------------------------
void GmVDeadTimeDUList::Clean(G4double time)
{
  GmVDeadTimeDUList::iterator ite;
  std::vector<GmVDeadTimeDUList::iterator> vite;
  for( ite = begin(); ite != end(); ite++ ){
    if( ((*ite).second).time < time ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmVDeadTimeDUList::Clean  deleting dead time " << ((*ite).second).time << " < " << time << " ID " << (*ite).first << G4endl;
#endif
      vite.push_back( ite );
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmVDeadTimeDUList::Clean  not deleting dead time " << ((*ite).second).time << " > " << time << " ID " << (*ite).first << G4endl;
#endif
    }
  }

  std::vector<GmVDeadTimeDUList::iterator>::iterator itev;
  for( itev = vite.begin(); itev != vite.end(); itev++ ){
    erase(*itev);
  }
}
 
//----------------------------------------------------------------------
G4bool GmVDeadTimeDUList::FindDetUnit( GmHit* hit, G4double deadTime )
{
  unsigned long long du = hit->GetDetUnitID();
  G4double hitTime = hit->GetTime();
  
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit Number of DUs= " << size() << G4endl;
#endif
  du = du/theNShift;
  
  iterator ite = find(du);
  if( ite != end() ) {
    G4double deadHitTime = ((*ite).second).time; 
    if(deadHitTime < hitTime &&  deadHitTime+deadTime >= hitTime && hit->GetEventID() != ((*ite).second).eventID ) {
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit du dead " << du << " in time " << deadHitTime << " > " << hitTime << G4endl;
#endif
      return true;
      } else {
	G4double deadHitTime2 = ((*ite).second).time;
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit du not dead " << du << " out of time " << deadHitTime2 << " <= " << hitTime <<  " || " << deadHitTime2+deadTime << " > " << hitTime << " || eventID " << hit->GetEventID() << " = " << ((*ite).second).eventID << G4endl;
#endif
    }
  }
   
  return false;
}
