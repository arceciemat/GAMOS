#include "GmPositionUserVolumes.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"

//------------------------------------------------------------------------
GmPositionUserVolumes::GmPositionUserVolumes()
{
  //  G4cout << " GmPositionUserVolumes::GmPositionUserVolumes " << G4endl;
  //  theName = "GmPositionUserVolumes";
}

//------------------------------------------------------------------------
GmPositionUserVolumes::~GmPositionUserVolumes()
{
}

//------------------------------------------------------------------------
std::vector<GVSTouchableInfo*> GmPositionUserVolumes::AddTouchableInfos( const std::vector<G4String>& params ) const
{
  
  std::vector<GVSTouchableInfo*> tinfos;
  GVSTouchableInfo* tinfo;

  unsigned int iSolid = 6;
  if( params[1] == "BOX"  
      || params[1] == "TUBE" 
      || params[1] == "TUBS" 
      || params[1] == "CONE" 
      || params[1] == "CONS" 
      || params[1] == "SPHERE" 
      || params[1] == "ORB" 
      || params[1] == "ELLIPSOID" )  { // using old format
    G4Exception("GmPositionUserVolumes::AddTouchableInfos",
		"You are using the old format to create a User volume",
		JustWarning,
		"Please use new one (read User's Guide)");
    iSolid = 1;
  }
  
  if( params.size() <= iSolid ) {
    G4Exception("GmPositionUserVolumes::AddTouchableInfos",
		"Too few arguments to create a User volume",
		FatalErrorInArgument,
		"Please read User's Guide");
  }

  if( iSolid == 6 ) {
    if( params[6] == "BOX" ){
      CheckNoSolidParams(params[6],10,params.size());
      G4VSolid* solid = new G4Box( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]) );
      tinfo = AddVolume( solid, params, iSolid );

    } else if( params[6] == "TUBE" ){
      CheckNoSolidParams(params[6],10,params.size());
      G4VSolid* solid = new G4Tubs( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), 0., 360.*CLHEP::deg );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "TUBS" ){
      CheckNoSolidParams(params[6],12,params.size());
      G4VSolid* solid = new G4Tubs( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "CONE" ){
      CheckNoSolidParams(params[6],12,params.size());
      G4VSolid* solid = new G4Cons( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), 0., 360.*CLHEP::deg );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "CONS" ){
      CheckNoSolidParams(params[6],14,params.size());
      G4VSolid* solid = new G4Cons( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), GmGenUtils::GetValue(params[12]), GmGenUtils::GetValue(params[13]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "ORB" ){
      CheckNoSolidParams(params[6],8,params.size());
      G4VSolid* solid = new G4Orb( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "SPHERE" ){
      CheckNoSolidParams(params[6],13,params.size());
      G4VSolid* solid = new G4Sphere( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]) , GmGenUtils::GetValue(params[11]) , GmGenUtils::GetValue(params[12]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[6] == "ELLIPSOID" ){
      CheckNoSolidParams(params[6],12,params.size());
      G4VSolid* solid = new G4Ellipsoid( "DUMMY_VOLUME", GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else {
      G4Exception("GmPositionUserVolumes::SetParams"," The only solid types currently supported are: BOX, TUBE, TUBS, CONE, CONS, SPHERE, ORB, ELLIPSOID",FatalErrorInArgument,G4String("You have used solid type = " + params[6]).c_str());
    }

  } else if( iSolid == 1 ) {
    if( params[1] == "BOX" ){
      CheckNoSolidParams(params[1],11,params.size());
      G4VSolid* solid = new G4Box( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "TUBE" ){
      CheckNoSolidParams(params[1],11,params.size());
      G4VSolid* solid = new G4Tubs( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), 0., 360.*CLHEP::deg );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "TUBS" ){
      CheckNoSolidParams(params[1],13,params.size());
      G4VSolid* solid = new G4Tubs( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), GmGenUtils::GetValue(params[12]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "CONE" ){
      CheckNoSolidParams(params[1],13,params.size());
      G4VSolid* solid = new G4Cons( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), GmGenUtils::GetValue(params[12]), 0., 360.*CLHEP::deg );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "CONS" ){
      CheckNoSolidParams(params[1],15,params.size());
      G4VSolid* solid = new G4Cons( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), GmGenUtils::GetValue(params[12]), GmGenUtils::GetValue(params[13]), GmGenUtils::GetValue(params[14]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "ORB" ){
      CheckNoSolidParams(params[1],9,params.size());
      G4VSolid* solid = new G4Orb( params[0], GmGenUtils::GetValue(params[8]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "SPHERE" ){
      CheckNoSolidParams(params[1],14,params.size());
      G4VSolid* solid = new G4Sphere( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]) , GmGenUtils::GetValue(params[12]) , GmGenUtils::GetValue(params[13]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else if( params[1] == "ELLIPSOID" ){
      CheckNoSolidParams(params[1],13,params.size());
      G4VSolid* solid = new G4Ellipsoid( params[0], GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]), GmGenUtils::GetValue(params[10]), GmGenUtils::GetValue(params[11]), GmGenUtils::GetValue(params[12]) );
      tinfo = AddVolume( solid, params, iSolid );
      
    } else {
      G4Exception("GmPositionUserVolumes::SetParams"," The only solid types currently supported are: BOX, TUBE, TUBS, CONE, CONS, SPHERE, ORB, ELLIPSOID",FatalErrorInArgument,G4String("You have used solid type = " + params[1]).c_str());
    }
  }
  
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmPositionUserVolumes volume added " << params[0] << " of solid type " << params[iSolid] << G4endl;
#endif
  tinfos.push_back( tinfo );

  return tinfos;
}



//-------------------------------------------------------------------
void GmPositionUserVolumes::CheckNoSolidParams( const G4String& solidType, const unsigned int NoParamExpected, const unsigned int NoParam ) const
{
  if( NoParamExpected != NoParam ) {
    G4cerr << "GmPositionUserVolumes::CheckNoSolidParams, solid type " << solidType << " should have " << NoParamExpected << " parameters, and it has " << NoParam << G4endl;
    G4Exception("GmPositionUserVolumes::CheckNoSolidParams",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }
}

//------------------------------------------------------------------------
GVSTouchableInfo* GmPositionUserVolumes::AddVolume( G4VSolid* solid, const std::vector<G4String>& params, unsigned int iSolid ) const
{
  GVSTouchableInfo* tinfo = new GVSTouchableInfo;
  tinfo->name = solid->GetName();
  tinfo->solid = solid;
  G4RotationMatrix rotmat;
  if( iSolid == 1 ) {
    tinfo->pos = G4ThreeVector( GmGenUtils::GetValue(params[2]), GmGenUtils::GetValue(params[3]), GmGenUtils::GetValue(params[4]) );
  rotmat.rotateX( GmGenUtils::GetValue(params[5]) );
  rotmat.rotateY( GmGenUtils::GetValue(params[6]) );
  rotmat.rotateY( GmGenUtils::GetValue(params[7]) );
  }else if( iSolid == 6 ) {
    tinfo->pos = G4ThreeVector( GmGenUtils::GetValue(params[0]), GmGenUtils::GetValue(params[1]), GmGenUtils::GetValue(params[2]) );
    rotmat.rotateX( GmGenUtils::GetValue(params[3]) );
    rotmat.rotateY( GmGenUtils::GetValue(params[4]) );
    rotmat.rotateY( GmGenUtils::GetValue(params[5]) );
  }

  tinfo->rotmat = rotmat;
  
  return tinfo;
}
