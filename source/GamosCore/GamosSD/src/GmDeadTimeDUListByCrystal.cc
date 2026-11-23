#include "GamosCore/GamosSD/include/GmDeadTimeDUListByCrystal.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "globals.hh"


//----------------------------------------------------------------------
GmDeadTimeDUListByCrystal::GmDeadTimeDUListByCrystal(G4String )
{
  theNShift = 1;
}


//----------------------------------------------------------------------
void GmDeadTimeDUListByCrystal::AddDetUnit( GmHit* hit )
{
  DTHitInfo hinfo;
  hinfo.time = hit->GetTime();
  hinfo.eventID = hit->GetEventID();
  (*this)[hit->GetDetUnitID()] = hinfo;
}
