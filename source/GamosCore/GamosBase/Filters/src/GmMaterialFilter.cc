#include "GmMaterialFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

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
    if( BaseVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptStep 1 " << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmMaterialFilter::AcceptStep 0 " << G4endl;
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
