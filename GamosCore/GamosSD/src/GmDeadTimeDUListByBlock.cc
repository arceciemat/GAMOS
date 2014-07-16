#include "GamosCore/GamosSD/include/GmDeadTimeDUListByBlock.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


//----------------------------------------------------------------------
GmDeadTimeDUListByBlock::GmDeadTimeDUListByBlock(G4String sdtype)
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theNShift = G4int(parmgr->GetNumericValue("SD:DeadTimeDUListByBlock:NShift:"+sdtype, 100));

  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:DeadTimeDUListByBlock:NAncestors:"+sdtype, 2));
  theNShift = G4int(pow(double(theNShift),int(NAncestors-1)));
  G4cout << " DeadTimeDUListByBlock: NShift " << theNShift << G4endl;

}


//----------------------------------------------------------------------
void GmDeadTimeDUListByBlock::AddDetUnit( GmHit* hit )
{
  DTHitInfo hinfo;
  hinfo.time = hit->GetTime();
  hinfo.eventID = hit->GetEventID();
  (*this)[hit->GetDetUnitID()/theNShift] = hinfo;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmDeadTimeDUListByBlock::AddDetUnit in Block " << hit->GetDetUnitID()/theNShift << " time " <<  hit->GetTime() << " N DU= " << size() << G4endl;
#endif
}
