#include "GmTouchableFilterRegion.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterRegion::GmTouchableFilterRegion()
{
  theRegions.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterRegion::~GmTouchableFilterRegion()
{ 
  theRegions.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterRegion::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterRegion::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  if( theRegions.find(touch->GetVolume()->GetLogicalVolume()->GetRegion()) != theRegions.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterRegion::AcceptTouchable TRUE return 1 " << touch->GetVolume()->GetLogicalVolume()->GetRegion()->GetName() << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmTouchableFilterRegion::AcceptTouchable return 0 " << touch->GetVolume()->GetLogicalVolume()->GetRegion()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterRegion::show()
{
  G4cout << "----GmTouchableFilterRegion volume list------"<<G4endl; 
  std::set<G4Region*>::const_iterator iter;
  for ( iter = theRegions.begin(); iter != theRegions.end(); iter++ ){
    G4cout << "GmTouchableFilterRegion volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterRegion::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterRegion::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  G4RegionStore* regions = G4RegionStore::GetInstance();  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    for( unsigned int jj = 0; jj < regions->size(); jj++ ){
      G4Region* reg = (*regions)[jj];
      if( GmGenUtils::AreWordsEquivalent(params[ii],reg->GetName()) ){
	theRegions.insert(reg);
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << "GmTouchableFilterRegion::SetParameters add parameter " << (*regions)[jj]->GetName() << G4endl;
#endif
      }
    }
  }
}
