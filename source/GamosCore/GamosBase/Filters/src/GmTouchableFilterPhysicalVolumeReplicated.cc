#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterPhysicalVolumeReplicated::GmTouchableFilterPhysicalVolumeReplicated()
{
  theVPhysicalVolumes.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterPhysicalVolumeReplicated::~GmTouchableFilterPhysicalVolumeReplicated()
{ 
  theVPhysicalVolumes.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterPhysicalVolumeReplicated::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicated::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  G4VPhysicalVolume* pv = touch->GetVolume();
  G4String pvName = pv->GetName() + ":" + GmGenUtils::ftoa(pv->GetCopyNo());
  if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicated::AcceptTouchable TRUE return 1 " << pvName << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicated::AcceptTouchable return 0 " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterPhysicalVolumeReplicated::show()
{
  G4cout << "----GmTouchableFilterPhysicalVolumeReplicated volume list------"<<G4endl; 
  std::set<G4String>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << "GmTouchableFilterPhysicalVolumeReplicated volume " << (*iter) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterPhysicalVolumeReplicated::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterPhysicalVolumeReplicated::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterPhysicalVolumeReplicated::SetParameters parameter " << params[ii] << G4endl;
#endif
    std::vector<G4String> volumes = GmGeometryUtils::GetInstance()->GetPhysicalVolumeNames( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theVPhysicalVolumes.insert( volumes[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterPhysicalVolumeReplicated::SetParameters add parameter " << volumes[jj] << G4endl;
#endif
     }
  }
}
