#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"

#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmHit.hh"
#include "GmSDVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

//---------------------------------------------------------------------
GmDetUnitIDBuilderFromAncestors::GmDetUnitIDBuilderFromAncestors( G4String sdType ) : GmDetUnitIDBuilder()
{
  G4cout << " GmDetUnitIDBuilderFromAncestors sdType " << sdType << G4endl;
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  if( parmgr->IsParameterInScript("SD:DetUnitID:NShift") ) {
    theNShift = G4int(parmgr->GetNumericValue("SD:DetUnitID:NShift", 100));
  } else {
    theNShift = G4int(parmgr->GetNumericValue("SD:DetUnitID:NShift:"+sdType, 100));
  }
  if( parmgr->IsParameterInScript("SD:DetUnitID:NAncestors") ) {
    theNAncestors = G4int(parmgr->GetNumericValue("SD:DetUnitID:NAncestors", 3));
  } else {
    theNAncestors = G4int(parmgr->GetNumericValue("SD:DetUnitID:NAncestors:"+sdType, 3));
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmDetUnitIDFromAncestors theNAncestors= " << theNAncestors << " theNShift" << theNShift << G4endl;
#endif
}

//---------------------------------------------------------------------
GmDetUnitIDBuilderFromAncestors::~GmDetUnitIDBuilderFromAncestors(){}

//---------------------------------------------------------------------
unsigned long long GmDetUnitIDBuilderFromAncestors::BuildID( G4Step* aStep )
{
  unsigned long long id = 0;

  G4TouchableHistory* touch = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());

  if( touch->GetHistoryDepth() < theNAncestors ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) {
      G4cerr << "!! WARNING GmDetUnitIDBuilderFromAncestors::BuildID: asking for " << theNAncestors << " ancestors, while there is only " << touch->GetHistoryDepth() << G4endl;
      G4cerr << "Use command :  /gamos/setParam SD:DetUnitID:NAncestors " << touch->GetHistoryDepth() << G4endl;
    }
#endif
    for( int ii = 0; ii < touch->GetHistoryDepth(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cerr << ii << " : " << touch->GetVolume( ii )->GetName() << G4endl;
#endif
    } 
  }

  for( int ii = 0; ii < touch->GetHistoryDepth(); ii++ ){
    if( ii < theNAncestors ) {
      G4VPhysicalVolume* pv = touch->GetVolume( ii );
      //      id += unsigned long long(pow(theNShift,ii)) * pv->GetCopyNo();
      id += (unsigned long long)pow(double(theNShift),int(ii)) * touch->GetReplicaNumber(ii);
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) G4cout << "GmDetUnitIDBuilderFromAncestors::Building ID: " << id << " ReplicaN " << touch->GetReplicaNumber(ii) << " name " << pv->GetName() <<" " << G4int(pow(double(theNShift),int(ii))) << " " << G4endl;
#endif
    }
  }

  return id;

}
