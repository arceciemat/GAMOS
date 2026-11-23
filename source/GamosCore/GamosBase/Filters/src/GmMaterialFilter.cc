#include "GmMaterialFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"

//--------------------------------------------------------------------
GmMaterialFilter::GmMaterialFilter(G4String name)
  :GmVFilter(name)
{
  theMaterials.clear();
}

//--------------------------------------------------------------------
GmMaterialFilter::~GmMaterialFilter()
{ 
  theMaterials.clear();
}

//--------------------------------------------------------------------
G4bool GmMaterialFilter::AcceptStep(const G4Step* aStep)
{
  G4Material* mate = (aStep->GetPreStepPoint()->GetMaterial());
  if( theMaterials.find(mate) != theMaterials.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptStep 1 " << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptStep 0 " << G4endl;
#endif
  return FALSE;
}

//--------------------------------------------------------------------
G4bool GmMaterialFilter::AcceptTrack(const G4Track* aTrack)
{
  G4Material* mate = (aTrack->GetNextMaterial());
  if( theMaterials.find(mate) != theMaterials.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptTrack 1 " << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptTrack 0 " << G4endl;
#endif
  return FALSE;
}

//--------------------------------------------------------------------
G4bool GmMaterialFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  G4Material* mate = GetMateFromPV( GmGeometryUtils::GetInstance()->GetPVFromPos(aTrack->GetPosition()) );
  if( theMaterials.find(mate) != theMaterials.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptTrack 1 " << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptTrack 0 " << G4endl;
#endif
  return FALSE;
}


//--------------------------------------------------------------------
void GmMaterialFilter::show()
{
  G4cout << "----G4MaterialFilter volume list------"<<G4endl;
  std::set<G4Material*>::const_iterator iter;
  for ( iter = theMaterials.begin(); iter != theMaterials.end(); iter++ ){
    G4cout << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//--------------------------------------------------------------------
void GmMaterialFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4Exception("GmMaterialFilter::SetParameters"
		,"There should be at least one parameter: MATERIAL_NAME"
		,FatalErrorInArgument
		,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4Material*> mates = GmGeometryUtils::GetInstance()->GetMaterials( params[ii], true );
    for(unsigned int jj = 0; jj < mates.size(); jj++ ){ 
      theMaterials.insert( mates[jj] );
    }
  }
}

//-------------------------------------------------------                                                
G4Material* GmMaterialFilter::GetMateFromPV( G4VPhysicalVolume* pv )
{
  // needs special treatment if track origin was in parameterised volume                                          
  if( !pv->IsParameterised() ) {
    return pv->GetLogicalVolume()->GetMaterial();
  } else {
    return ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(pv->GetCopyNo(),pv);
  }

}
