#include "GmTouchableFilterLogicalVolume.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolume::GmTouchableFilterLogicalVolume()
{
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolume::~GmTouchableFilterLogicalVolume()
{ 
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterLogicalVolume::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolume::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  if( theLogicalVolumes.find(touch->GetVolume()->GetLogicalVolume()) != theLogicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) {
      G4cout << " GmTouchableFilterLogicalVolume::AcceptTouchable TRUE return 1 " << touch->GetVolume()->GetLogicalVolume()->GetName() << G4endl;
      std::set<G4LogicalVolume*>::const_iterator ite;
      for( ite=  theLogicalVolumes.begin(); ite != theLogicalVolumes.end(); ite++ ) {
	G4cout << " GmTouchableFilterLogicalVolume::AcceptTouchable looking for  " << (*ite)->GetName() << G4endl;
      }
    }
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) {
      G4cout << " GmTouchableFilterLogicalVolume::AcceptTouchable FALSE return 0 " << touch->GetVolume()->GetLogicalVolume()->GetName() << G4endl;
      std::set<G4LogicalVolume*>::const_iterator ite;
      for( ite=  theLogicalVolumes.begin(); ite != theLogicalVolumes.end(); ite++ ) {
	G4cout << " GmTouchableFilterLogicalVolume::AcceptTouchable looking for  " << (*ite)->GetName() << G4endl;
      }
    }
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolume::show()
{
  G4cout << "----GmTouchableFilterLogicalVolume volume list------"<<G4endl; 
  std::set<G4LogicalVolume*>::const_iterator iter;
  for ( iter = theLogicalVolumes.begin(); iter != theLogicalVolumes.end(); iter++ ){
    G4cout << "GmTouchableFilterLogicalVolume volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolume::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterLogicalVolume::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4LogicalVolume*> volumes = GmGeometryUtils::GetInstance()->GetLogicalVolumes( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theLogicalVolumes.insert( volumes[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << "GmTouchableFilterLogicalVolume::SetParameters add parameter " << volumes[jj]->GetName() << G4endl;
#endif
     }
  }
}
