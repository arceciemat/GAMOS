#include "GmTouchablePhantomStructure.hh"
#include "GmReadPhantomStMgr.hh"
#include "GmRegularParamUtils.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

//---------------------------------------------------------------------
GmTouchablePhantomStructure::GmTouchablePhantomStructure()
{
  theRegularUtils = GmRegularParamUtils::GetInstance();
  theStructMgr = GmReadPhantomStMgr::GetInstance();
  if( theStructMgr->GetStructs().size() == 0 ) {
    G4Exception("GmTouchablePhantomStructure::GmTouchablePhantomStructure",
                "",
                FatalErrorInArgument,
                " No structure found in G4 DICOM file");
  }

}

//---------------------------------------------------------------------
GmTouchablePhantomStructure::~GmTouchablePhantomStructure()
{ 
}

//---------------------------------------------------------------------
G4bool GmTouchablePhantomStructure::AcceptTouchable(const G4VTouchable* touch)
{
  G4VPhysicalVolume* pv = touch->GetVolume();
  if( !pv ) {
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << " GmTouchablePhantomStructure::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }

  if( GmGeometryUtils::GetInstance()->IsPhantomVolume( pv ) ){
    G4int idx = touch->GetReplicaNumber();
    if( idx != -1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << pv->GetCopyNo() << " GmTouchablePhantomStructure idx " << idx << G4endl;
#endif
      if( CheckIndex(idx) ) {
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) )   G4cout << " GmTouchablePhantomStructure idx " << idx << " TRUE " << G4endl;   
#endif
	return TRUE;
      }else {
 #ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) ) G4cout << " GmTouchablePhantomStructure idx " << idx << " FALSE " << G4endl;     
#endif
	return FALSE;
      }
    }
  }

  return FALSE;
}

//---------------------------------------------------------------------
G4bool GmTouchablePhantomStructure::CheckIndex( G4int idx ) 
{
  //split index
  std::set<size_t> StIDList = theStructMgr->GetStIDList(idx);
  for( std::set<size_t>::const_iterator ite = StIDList.begin(); ite != StIDList.end(); ite++ ) {
    G4int idx1 = *ite;
    if( theIndices.find(idx1) != theIndices.end() ) {
      return true;
    }
  }

  return false;

}
//---------------------------------------------------------------------
void GmTouchablePhantomStructure::show()
{
  G4cout << "----GmTouchablePhantomStructure index list------"<<G4endl; 
  std::set<G4int>::const_iterator ite;
  for( ite = theIndices.begin(); ite != theIndices.end(); ite++ ){
    G4cout << "GmTouchablePhantomStructure index " << (*ite) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchablePhantomStructure::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchablePhantomStructure::SetParameters","There should be at least one parameter: PARTICLE_NAME"
		,FatalErrorInArgument,"There is no parameter");
  }
  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4int> stids = theStructMgr->GetStIDFromPhysVolName(params[ii]);
    for( size_t jj = 0; jj < stids.size(); jj++ ) {
      theIndices.insert( stids[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(-debugVerb) )  G4cout << ii << " GmTouchablePhantomStructure theIndices " << stids[jj] << G4endl;
#endif
    }
  }

}

