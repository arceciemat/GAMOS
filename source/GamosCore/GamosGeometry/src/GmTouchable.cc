#include "GmTouchable.hh"
#include "GmGeomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <vector>
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Geometry/Transform3D.h"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4VTouchable.hh"
#include "G4Point3D.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmTouchable::GmTouchable( std::vector<G4VPhysicalVolume*> vpv, std::vector<G4int>& ancestorsCopyNo )
{
  BuildTouchable( vpv, ancestorsCopyNo );
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void GmTouchable::BuildTouchable( std::vector<G4VPhysicalVolume*> vpv, std::vector<G4int>& ancestorsCopyNo )
{
  if( vpv.size() == 0 ) {
    G4cerr << "!!!EXITING:GmTouchable::GmTouchable: std::vector<G4VPhysicalVolume*> has dimension 0 " << G4endl;
    exit(0);
  }

  //------ physical volume is the first one in vector, others are ancestors of it
  G4VPhysicalVolume* pv = vpv[0];
  bool isReplica = pv->IsReplicated();
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "GmTouchable::GmTouchable " << pv->GetName()  << G4endl;
#endif

  //----- name
  theName = pv->GetName();
  //----- long name: for efficiency reasons, build it when looping the PV's

  //----- copyNo: if Placement GetCopyNo
  //              if Replica, get it from ancestorsCopyNo
  if( !isReplica ) {
    theCopyNo = pv->GetCopyNo();
  } else {
    theCopyNo = ancestorsCopyNo[0];
  }

  // theVisCateg
  //----- materialName
  if( !pv->IsParameterised() ) {
    theMaterialName = pv->GetLogicalVolume()->GetMaterial()->GetName();
  } else {
    theMaterialName = ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(theCopyNo,pv)->GetName();
  }
  //----- solid 
  theSolid = pv->GetLogicalVolume()->GetSolid();

  //--- One Exception to the rule: if it is a division of a tubs or cons along kRho, each copy has different solid parameters
  if( isReplica ) {
    EAxis axis;
    G4int nReplicas;
    G4double width;
    G4double offset;
    G4bool consuming;
    pv->GetReplicationData(axis, nReplicas, width, offset, consuming);
    if( axis == kRho ) {
      G4cerr << " !!!EXITING at GmTouchable::GmTouchable: division along radius not supported yet !! Please contact Pedro.Arce@cern.ch" << G4endl;
      exit(1);
      /*t      if( theSolid->getType() == "TUBS" ) {
  theSolid->setParam( HSolTubsRMin ) = 
      */
    }
  }

  //------ Get local and global traslation and rotation
  //----- Loop over ancestors to get the global traslation and rotation
  //----- Build the long name
  G4int nAnces = vpv.size();
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "nAnces Build " << nAnces << G4endl;
#endif
  G4int ii;

  G4Transform3D globalTrans = G4Transform3D();
  theLongName = "";
  G4String ancestorName;
  for( ii = 0; ii < nAnces; ii++ ) {
    G4VPhysicalVolume* pv2 = vpv[ii];
    //    G4cout << ii << " trans pv " << pv << G4endl;
    ancestorName = "/" + pv2->GetName();
    G4int copyNoAncestor;
    if( !pv2->IsReplicated() ) {
      copyNoAncestor = pv2->GetCopyNo();
    } else {
      copyNoAncestor = ancestorsCopyNo[ii];
    }
    copyNoAncestor = ancestorsCopyNo[ii];
    //    G4cout << "  ancestorsCopyNo[ii] " <<  ancestorsCopyNo[ii] << " pv2->GetCopyNo() " << pv2->GetCopyNo() << G4endl;
    //    char chartmp[10];
    //gcvt( copyNoAncestor, 10, chartmp );
   // ancestorName += G4String(":") + G4String(chartmp);
		ancestorName += G4String(":") + GmGenUtils::itoa(copyNoAncestor);
		theLongName =  ancestorName + theLongName;

    G4Transform3D trans = CalculateTransformation( pv2, ancestorsCopyNo[ii] );
    globalTrans = trans * globalTrans;
    //    G4cout << " trans " << trans.getTranslation() << G4endl;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GLOBAL TRANSFORMATION FOR " << ancestorName << G4endl
	   << " TRANS " << globalTrans.getTranslation() << " ROT " << globalTrans.getRotation() << G4endl
	   << " CENTRE " << G4Point3D(0.,0.,0.).transform( globalTrans ) << G4endl
	   << " X " << G4Point3D(1.,0.,0.).transform( globalTrans ) << G4endl
	   << " Y " << G4Point3D(0.,1.,0.).transform( globalTrans ) << G4endl
	   << " Z " << G4Point3D(0.,0.,1.).transform( globalTrans ) << G4endl;
#endif
    if( ii == 0 ){
      theLocalPos = trans.getTranslation();
      theLocalRotMat = trans.getRotation();
    }
    //-    G4cout << ii << G4endl;
   //  DdUtils::dumpTransform( trans, (char*)("transform rot ") ); 
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << "transform rot " << trans.getRotation() << " transl " << trans.getTranslation() << G4endl;
#endif
    //  Utils::dumpTransform( globalTrans, "global transform rot " );
  }
  theGlobalPos = globalTrans.getTranslation();
  theGlobalRotMat = globalTrans.getRotation();

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmTouchable::GmTouchable( const G4VTouchable* g4touch )
{
  std::vector<G4VPhysicalVolume*> vpv;
  std::vector<G4int> ancestorsCopyNo;

  G4int nAncest = g4touch->GetHistoryDepth();
  for( G4int ii = 0; ii <= nAncest; ii++ ) {
    vpv.push_back(g4touch->GetVolume(ii));
    ancestorsCopyNo.push_back(g4touch->GetCopyNumber(ii));
  }

  BuildTouchable( vpv, ancestorsCopyNo );
  return;

  G4VPhysicalVolume* pv = g4touch->GetVolume();
  //  G4cout << " GmTouchable::GmTouchable " << g4touch << " " << pv << G4endl;
  if( !pv ) return;

  //----- name
  theName = pv->GetName();
  //----- long name: for efficiency reasons, build it when looping the PV's

  //----- copyNo
  theCopyNo = pv->GetCopyNo();

  // theVisCateg
  //----- materialName
  if( !pv->IsParameterised() ) {
    theMaterialName = pv->GetLogicalVolume()->GetMaterial()->GetName();
  } else {
    theMaterialName = ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(theCopyNo,pv)->GetName();
  }

  //----- solid 
  theSolid = pv->GetLogicalVolume()->GetSolid();

  //--- One Exception to the rule: if it is a division of a tubs or cons along kRho, each copy has different solid parameters
  bool isReplica = pv->IsReplicated();
  if( isReplica ) {
    EAxis axis;
    G4int nReplicas;
    G4double width;
    G4double offset;
    G4bool consuming;
    pv->GetReplicationData(axis, nReplicas, width, offset, consuming);
    if( axis == kRho ) {
      G4cerr << " !!!EXITING at GmTouchable::GmTouchable: division along radius not supported yet !! Please contact Pedro.Arce@cern.ch" << G4endl;
      exit(1);
      /*t      if( theSolid->getType() == "TUBS" ) {
  theSolid->setParam( HSolTubsRMin ) = 
      */
    }
  }

  //------ Get local and global traslation and rotation
  //----- Loop over ancestors to get the global traslation and rotation
  //----- Build the long name
  G4int ii;

  G4Transform3D globalTrans = G4Transform3D();
  theLongName = "";
  G4String ancestorName;
  G4int nAnces = g4touch->GetHistoryDepth();
  //  for( ii = 0; ii < nAnces-1; ii++ ) {
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "nAnces " << nAnces << G4endl;
#endif
  for( ii = 0; ii < nAnces; ii++ ) {
    //    g4touch->MoveUpHistory();
    G4VPhysicalVolume* pv2 = g4touch->GetVolume(ii);
    //-    G4VPhysicalVolume* pv2 = g4touch->GetVolume(ii+1);
    //-    G4cout << " trans pv2 " << pv2->GetName() << G4endl;
    ancestorName = "/" + pv2->GetName();
    int copyNoAncestor;
    if( !pv2->IsReplicated() ) {
      copyNoAncestor = pv2->GetCopyNo();
    } else {
      copyNoAncestor = -1;
    }
    //    char chartmp[10];
//    gcvt( copyNoAncestor, 10, chartmp );
 //   ancestorName += G4String(":") + G4String(chartmp);
		ancestorName += G4String(":") + GmGenUtils::itoa(copyNoAncestor);

    theLongName =  ancestorName + theLongName;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << ii << "ancestorsName " << ancestorName << " theLongName " << theLongName << G4endl;
#endif

    G4Transform3D trans;
    if( pv2->GetRotation() != 0 ) {
      trans = G4Transform3D( *(pv2->GetRotation()), pv2->GetObjectTranslation() );
    } else {
      //      G4cout << " no rotation for volume " << pv2->GetName() << "  " << pv2 << G4endl;
      trans = G4Transform3D( G4RotationMatrix(), pv2->GetObjectTranslation() );
    }  
    // CalculateTransformation( pv2, copyNoAncestor );
    globalTrans = trans * globalTrans;
    if( ii == 0 ){
      theLocalPos = trans.getTranslation();
      theLocalRotMat = trans.getRotation();
    }
    //-    G4cout << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout<< "global transform rot " << trans.getRotation() << " transl " << trans.getTranslation() << G4endl;
#endif
    //  DdUtils::dumpTransform( globalTrans, "global transform rot " );
  }
  theGlobalPos = globalTrans.getTranslation();
  theGlobalRotMat = globalTrans.getRotation();

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmTouchable::~GmTouchable()
{
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4Transform3D GmTouchable::CalculateTransformation( G4VPhysicalVolume* pv, G4int index )
{
  G4Transform3D trans;
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << pv->GetName() << " index " << index << G4endl;
#endif
  //-  G4cout << pv->GetName() << "local pos " <<  pv->GetObjectTranslation() <<G4endl; 
  //----- If it is simple placement, just get the translation and 
  if( ! pv->IsReplicated() ) {
    if( pv->GetMotherLogical() == 0 ) { //World volume
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << " No replica: WORLD volume " << G4endl;
#endif
      trans = G4Transform3D();
    }else  if( pv->GetRotation() == 0 ) { 
      trans = G4Transform3D( G4RotationMatrix(), pv->GetObjectTranslation() );
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << " No replica: rotm 0 " << G4endl;
#endif
   }else {
      trans = G4Transform3D( pv->GetObjectRotationValue(), pv->GetObjectTranslation() );
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << " No replica: " << G4endl;
#endif
    }
    //----- If it is replica 
  } else if (pv->GetParameterisation() == 0 ){
    EAxis axis;
    G4int nReplicas;
    G4double width;
    G4double offset;
    G4bool consuming;
    pv->GetReplicationData(axis, nReplicas, width, offset, consuming);
    
    //---- Translation is calculated from index of replica
    //---- Rotation is 0 always, except if axis is kPhi     
    G4RotationMatrix rotm;
    G4ThreeVector pos(0.,1.,0.);  //is the one used by kRho, for example
    G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
    G4String sType = solid->GetEntityType();
    //-    G4cout << pv->GetName() << " mother pv " << pv->GetMother() << " sType " << sType << " axis " << axis << G4endl;
    switch (axis ){
    case kXAxis:
      pos.setY(0.);
      pos.setX( offset + width * (index+0.5) );
      break;
    case kYAxis:
      pos.setY(offset + width * (index+0.5));
      break;
    case kZAxis:
      pos.setY(0.);
      pos.setZ( offset + width * (index+0.5) );
      break;
    case kRho:
      //--- If it is a full cone of tube, position is at center, if it is section of cone or tube, position is at radius 
      if( sType == "G4Tubs" ) { 
	G4Tubs* solTubs = static_cast<G4Tubs*>(solid);
	//-G4cout << " kRho replica: delta Phi =" << solTubs->GetDeltaPhiAngle() <<G4endl;
	if(  solTubs->GetDeltaPhiAngle() != 360.*CLHEP::deg ) { 
	  //??depends on how you implement this parameterisation	  pos = G4ThreeVector(0.,0.,0.);
	  pos.setMag( offset + width * (index+0.5) );
        }
      } else if( sType == "G4Cons" ) { 
	G4Cons* solCons = static_cast<G4Cons*>(solid);
	if( solCons->GetDeltaPhiAngle() != 360.*CLHEP::deg ) { 
	  //??depends on how you implement this parameterisation	  pos = G4ThreeVector(0.,0.,0.);
	  pos.setMag( offset + width * (index+0.5) );
        } 
      } else {
	G4cerr << "!!!EXITING: GmTouchable::CalculateTransformation : replica along kRho for volume different than G4Tubs, G4Cons = " << sType << G4endl;
	exit(1);
      }
      break;
    case kPhi:
      // If it is a cone or tube section, centre is at half between minimum and maximum radii, rotation is that with which the section points to the center
      // If it is the full cone or tube (pathological case where number of divisions is 1) then centre is 0 and rotation identity
      if( sType == "G4Tubs" ) { 
	G4Tubs* solTubs = static_cast<G4Tubs*>(solid);
	//-	G4cout << " kPhi replica: delta Phi =" << solTubs->GetDeltaPhiAngle() <<G4endl;
	if( solTubs->GetDeltaPhiAngle() != 360.*CLHEP::deg ) { 
	  pos = G4ThreeVector(0.,0.,0.);
//o  pos.setMag( (solTubs->GetInnerRadius() + solTubs->GetOuterRadius()) / 2. );
	  G4double phi =  offset + width * (index+0.5);
	  //o	  pos.setPhi( phi );
	  rotm.rotateZ( phi ); 
        }
      } else if( sType == "G4Cons" ) { 
	G4Cons* solCons = static_cast<G4Cons*>(solid);
	if( solCons->GetDeltaPhiAngle() != 360.*CLHEP::deg ) { 
	  pos = G4ThreeVector(0.,0.,0.);
	  //o  pos.setMag( ( solCons->GetInnerRadiusMinusZ() + solCons->GetInnerRadiusPlusZ() + solCons->GetOuterRadiusMinusZ() + solCons->GetOuterRadiusPlusZ()) / 4. );
	  G4double phi =  offset + width * (index+0.5);
	  //o  pos.setPhi( phi );
	  rotm.rotateZ( phi ); 
        } 
      } else {
	G4cerr << "!!!EXITING: GmTouchable::CalculateTransformation : replica along kPhi for volume different than G4Tubs, G4Cons = " << sType << G4endl;
	exit(1);
      }
      break; 
    default:
      break;
    } 
    //-    G4cout << "replica of type " << axis << " solid type " << sType << G4endl;
    trans = G4Transform3D( rotm, pos );
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << " Replica:  " << G4endl;
#endif

    //----- If it is parameterisation
  } else {
   //--- ComputeTransformation without modifying pv
    G4PVParameterised* pvparam = (G4PVParameterised*)(pv);
    G4VPVParameterisation* param = pvparam->GetParameterisation();

    //create dummy PV just to extract the transformation
    G4RotationMatrix rm0;
    G4PVPlacement* pvDummy = new G4PVPlacement(&rm0, G4ThreeVector(), "DUMMY", pv->GetLogicalVolume(), 0, false, 0);

    param->ComputeTransformation( index, pvDummy );
    trans = G4Transform3D(*(pvDummy->GetRotation()), pvDummy->GetTranslation());
    delete pvDummy;

#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << " parameterisation:  " << G4endl;
#endif

  }

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmTouchable::CalculateTransformation " << index << " rotation " << trans.getRotation() << " translation " << trans.getTranslation() << G4endl;
#endif

  return trans;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
std::ostream& operator<<(std::ostream& os, const GmTouchable& touch) 
{
  os << "GmTouchable: " << touch.theLongName 
     << " solid type " << touch.theSolid->GetEntityType() << " material " << touch.theMaterialName
     << " Global position " << touch.theGlobalPos 
     << " rotation " << touch.theGlobalRotMat
     << " Local position " << touch.theLocalPos;

  return os;
}
