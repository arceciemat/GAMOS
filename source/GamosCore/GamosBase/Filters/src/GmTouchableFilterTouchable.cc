#include "GmTouchableFilterTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterTouchable::GmTouchableFilterTouchable()
{
  geomUtils = GmGeometryUtils::GetInstance();
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterTouchable::~GmTouchableFilterTouchable()
{ 
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterTouchable::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  for( unsigned int ii = 0; ii < theTouchableNames.size(); ii++){
    vpsi ancestors = theTouchableNames[ii];
    G4int sizV = ancestors.size();
    if( touch->GetHistoryDepth() < sizV ) continue;
    G4bool bFound = TRUE;
    for( G4int jj = 0; jj < sizV; jj++ ){
      G4VPhysicalVolume* pv = touch->GetVolume(jj);
      G4bool cn = !geomUtils->CheckPVCopyNo( pv, ancestors[jj].second ) ;
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable check " <<  pv->GetName() << " != " << ancestors[jj].first << " " 
	     << pv->GetCopyNo() << "- " << ancestors[jj].second 
	     << "?" << (pv->GetName() != ancestors[jj].first)
	     << "?" << G4int(cn)
	     <<G4endl;
#endif
      if( pv->GetName() != ancestors[jj].first ||
	  !geomUtils->CheckPVCopyNo( pv, ancestors[jj].second )  ){
	bFound = FALSE; 
	break; 
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return " << bFound << G4endl;
#endif
    if( bFound ) return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchable::AcceptTouchable return 0 " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchable::show()
{
  G4cout << "----GmTouchableFilterTouchable volume list------"<<G4endl; 
  std::vector<vpsi>::const_iterator iter;
  for ( iter = theTouchableNames.begin(); iter != theTouchableNames.end(); iter++ ){
    //    G4cout << "GmTouchableFilterTouchable volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchable::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterTouchable::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    vpsi ancestors = geomUtils->ExtractAncestorsRequested( params[ii] );
    theTouchableNames.push_back(ancestors);
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterTouchable::SetParameters add parameter " << params[ii] << G4endl;
#endif
  }
}
