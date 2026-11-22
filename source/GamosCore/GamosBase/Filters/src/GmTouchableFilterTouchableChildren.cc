#include "GmTouchableFilterTouchableChildren.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterTouchableChildren::GmTouchableFilterTouchableChildren()
{
  geomUtils = GmGeometryUtils::GetInstance();
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterTouchableChildren::~GmTouchableFilterTouchableChildren()
{ 
  theTouchableNames.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterTouchableChildren::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchableChildren::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  for( unsigned int ii = 0; ii < theTouchableNames.size(); ii++){
    vpsi ancestors = theTouchableNames[ii];
    G4int sizV = G4int(ancestors.size());
    if( touch->GetHistoryDepth() < sizV ) continue;
    G4bool bFound = TRUE;
    G4int bFirst = 0;
    unsigned int kk = 0;
    for( G4int jj = 0; jj < sizV; jj++ ){
      G4VPhysicalVolume* pv = touch->GetVolume(jj);
      if( pv->GetName() != ancestors[kk].first ||
	  geomUtils->CheckPVCopyNo( pv, ancestors[kk].second )  ){
	if( bFirst != 0 ) {
	  bFound = FALSE; 
	  break; 
	}
      } else { 
	bFirst =jj;
	kk++;
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchableChildren::AcceptTouchable TRUE return 1 " << bFound << G4endl;
#endif
    if( bFound ) return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterTouchableChildren::AcceptTouchable return 0 " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchableChildren::show()
{
  G4cout << "----GmTouchableFilterTouchableChildren volume list------"<<G4endl; 
  std::vector<vpsi>::const_iterator iter;
  for ( iter = theTouchableNames.begin(); iter != theTouchableNames.end(); iter++ ){
    //    G4cout << "GmTouchableFilterTouchableChildren volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterTouchableChildren::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterTouchableChildren::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    vpsi ancestors = geomUtils->ExtractAncestorsRequested( params[ii] );
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterTouchableChildren::SetParameters add parameter " << params[ii] << G4endl;
#endif
  }
}
