#include "GmTouchableFilterLogicalVolumeChildren.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolumeChildren::GmTouchableFilterLogicalVolumeChildren()
{
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolumeChildren::~GmTouchableFilterLogicalVolumeChildren()
{ 
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterLogicalVolumeChildren::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return FALSE, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  G4int siz = touch->GetHistoryDepth();
  for(G4int ii = 0; ii < siz; ii++ ){
    if( theLogicalVolumes.find(touch->GetVolume(ii)->GetLogicalVolume()) != theLogicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return TRUE " << touch->GetVolume(ii)->GetName() << G4endl;
#endif
      return TRUE;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return FALSE " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolumeChildren::show()
{
  G4cout << "----GmTouchableFilterLogicalVolumeChildren volume list------"<<G4endl; 
  std::set<G4LogicalVolume*>::const_iterator iter;
  for ( iter = theLogicalVolumes.begin(); iter != theLogicalVolumes.end(); iter++ ){
    G4cout << "GmTouchableFilterLogicalVolumeChildren volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolumeChildren::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterLogicalVolumeChildren::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4LogicalVolume*> volumes = GmGeometryUtils::GetInstance()->GetLogicalVolumes( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theLogicalVolumes.insert( volumes[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterLogicalVolumeChildren::SetParameters add parameter " << volumes[jj]->GetName() << G4endl;
#endif
     }
  }
}
