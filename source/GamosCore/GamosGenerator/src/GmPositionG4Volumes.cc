#include "GmPositionG4Volumes.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "G4VSolid.hh"

//------------------------------------------------------------------------
GmPositionG4Volumes::GmPositionG4Volumes()
{
}

//------------------------------------------------------------------------
std::vector<GVSTouchableInfo*> GmPositionG4Volumes::AddTouchableInfos( const std::vector<G4String>& params ) const
{
  std::vector<GVSTouchableInfo*> tinfos;

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( params[ii] , TRUE );
    if( touchs.size() == 0 ) {
      G4Exception("GmGenerDistPositionInG4Volumes::AddTouchableInfos",
		    "Wrong argument",
		    FatalErrorInArgument,
		  "No volume found in geometry, recheck the names " );
    }
    for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
      GVSTouchableInfo* tinfo = new GVSTouchableInfo;
      tinfo->name = (*ite)->GetLongName();
      tinfo->solid = (*ite )->GetSolid();
      tinfo->pos = (*ite)->GetGlobalPos();
      tinfo->rotmat = (*ite)->GetGlobalRotation();

#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(infoVerb) ) G4cout << tinfos.size() << " ADDING tinfo pos " <<  tinfo->pos << G4endl;
#endif
      tinfos.push_back(tinfo);
      delete *ite;
    }

#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionInG4Volumes volume added " << params[ii] << G4endl;
#endif
    //t CheckVolumeExists( params[ii] );
  }

  return tinfos;  
}
