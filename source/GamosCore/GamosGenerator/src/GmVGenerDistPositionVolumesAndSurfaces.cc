#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"

//------------------------------------------------------------------------
GmVGenerDistPositionVolumesAndSurfaces::GmVGenerDistPositionVolumesAndSurfaces()
{
 
  theName = "GmVGenerDistPositionVolumesAndSurfaces";

 //- theTouchable = new G4TouchableHistory();

  theGeomUtils = GmGeometryUtils::GetInstance();

  bOnlyVolume = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmVGenerDistPositionVolumesAndSurfaces:OnlyVolume",0));

}

//------------------------------------------------------------------------
GmVGenerDistPositionVolumesAndSurfaces::~GmVGenerDistPositionVolumesAndSurfaces()
{
}


//------------------------------------------------------------------------
void GmVGenerDistPositionVolumesAndSurfaces::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    G4Exception("GmVGenerDistPositionVolumesAndSurfaces::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"No volume defined: you have to add at least one, as the last argument in the command line setting this distribution ");
  }

  std::vector<GVSTouchableInfo*> tinfos = theOrigin->AddTouchableInfos( params );
  for(unsigned int jj=0; jj<theTouchableInfos.size(); jj++) { //gdl
    delete theTouchableInfos[jj];//gdl
  }//gdl
  theTouchableInfos.clear();
  theTotalObjectDimensions.clear();

  //--- Set total objects dimensions
  for( std::vector<GVSTouchableInfo*>::const_iterator ite = tinfos.begin(); ite != tinfos.end(); ite++ ) {
    theTouchableInfos.push_back( *ite );

    const G4VSolid* solid = (*ite)->solid;
    theTotalObjectDimensions.push_back(theObject->GetObjectDimension(solid) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::AddVolume adding volume " << (*ite)->name << " dimension " << theObject->GetObjectDimension(solid) << G4endl;
#endif
    unsigned int siz = theTotalObjectDimensions.size();
    if( siz > 1 ) theTotalObjectDimensions[siz-1] += theTotalObjectDimensions[siz-2];

    theObject->BuildSurfaceAreas( solid );
  }

}

//------------------------------------------------------------------------
G4ThreeVector GmVGenerDistPositionVolumesAndSurfaces::GeneratePosition( const GmParticleSource* )
{
  //--------- Select an object
  unsigned int siz = theTotalObjectDimensions.size();
  G4double rvol = CLHEP::RandFlat::shoot() * theTotalObjectDimensions[siz-1];
  //-  G4cout << "rvolshoot " << rvol << " " <<  theTotalObjectDimensions[siz-1] << G4endl;
  unsigned int io;
  for( io = 0; io < siz; io++ ){
    if( rvol <= theTotalObjectDimensions[io] ) {
      break;
    }
  }
   GVSTouchableInfo* currentTouchableInfo = theTouchableInfos[io];
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate in volume io " << io << " " << theTouchableInfos[io]->name << " rvol " << rvol << G4endl;
#endif
  
  G4ThreeVector pos;
  G4String volName;
  do {
    
    pos = theObject->GeneratePosition( currentTouchableInfo );
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate pos before trans " << pos << G4endl;
#endif
    
    pos = currentTouchableInfo->rotmat * pos;
    pos += currentTouchableInfo->pos;
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPositionVolumesAndSurfaces::Generate pos " << pos << " touchable global pos " <<currentTouchableInfo->pos  << G4endl;
#endif
    volName = theGeomUtils->BuildTouchableName( pos );
    
  } while(volName != currentTouchableInfo->name && bOnlyVolume );
    
  return pos;

}
