#include "GmGenerDistPositionInG4VolumesGeneral.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"

//------------------------------------------------------------------------
GmGenerDistPositionInG4VolumesGeneral::GmGenerDistPositionInG4VolumesGeneral()
{
  theName = "GmGenerDistPositionInG4VolumesGeneral";

  theTouchable = new G4TouchableHistory();

  theGeomUtils = GmGeometryUtils::GetInstance();

  SetWorldDimensions();
}

//------------------------------------------------------------------------
void GmGenerDistPositionInG4VolumesGeneral::SetParams( const std::vector<G4String>& params )
{
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    theVolumes.push_back( params[ii] );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4VolumesGeneral volume added " << params[ii] << G4endl;
#endif
    //t CheckVolumeExists( params[ii] );
  }

}

//------------------------------------------------------------------------
void GmGenerDistPositionInG4VolumesGeneral::SetWorldDimensions()
{
  G4LogicalVolume* worldLV = GmGeometryUtils::GetInstance()->GetTopLV();
  if( worldLV->GetSolid()->GetEntityType() != "G4Box" ){
    G4Box* world = static_cast<G4Box*>(worldLV->GetSolid());
    theWorldHalfX = world->GetXHalfLength();
    theWorldHalfY = world->GetYHalfLength();
    theWorldHalfZ = world->GetZHalfLength();
  } else if( worldLV->GetSolid()->GetEntityType() != "G4Tubs" ){
    G4Tubs* world = static_cast<G4Tubs*>(worldLV->GetSolid());
    theWorldHalfX = world->GetOuterRadius();
    theWorldHalfY = world->GetOuterRadius();
    theWorldHalfZ = world->GetZHalfLength();
  } else {
    G4Exception("GmGenerDistPositionInG4VolumesGeneral::SetWorldDimensions",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Only world shapes supported are G4Box and G4Tubs, your world is a " + worldLV->GetSolid()->GetEntityType() ).c_str());
  } 
}

//------------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionInG4VolumesGeneral::GeneratePosition( const GmParticleSource* )
{
  G4ThreeVector pos;
  G4bool inVolumes = false;
  G4int nt = 0;
  unsigned int siz = theVolumes.size();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4VolumesGeneral::Generate " << siz << G4endl;
#endif
  do {
    pos = G4ThreeVector( -theWorldHalfX+2*theWorldHalfX*CLHEP::RandFlat::shoot(), 
		      -theWorldHalfY+2*theWorldHalfY*CLHEP::RandFlat::shoot(),
		      -theWorldHalfZ+2*theWorldHalfZ*CLHEP::RandFlat::shoot() );
    G4String volName = theGeomUtils->BuildTouchableName( pos );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << nt << " GmGenerDistPositionInG4VolumesGeneral::Generate " << pos << " volume " << volName << G4endl;
#endif
    for( unsigned int ii = 0; ii < siz; ii++ ) {
      if( int(volName.find( theVolumes[ii] )) != -1 ) {
	inVolumes = true;
	break;
      }
    }
    nt++;
  } while( !inVolumes );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4VolumesGeneral::Generate pos " << pos << " " << nt << G4endl;
#endif
  return pos;
}
