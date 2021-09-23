#include "GmGenerDistPositionInMaterialPhantomVoxels.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4PhantomParameterisation.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4AffineTransform.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Geantino.hh"
#include "CLHEP/Random/RandFlat.h"

//------------------------------------------------------------------------
GmGenerDistPositionInMaterialPhantomVoxels::GmGenerDistPositionInMaterialPhantomVoxels()
{
  bInitialised = false;
}

//------------------------------------------------------------------------
void GmGenerDistPositionInMaterialPhantomVoxels::BuildVoxelPositions()
{
  
  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
  //---- Add the G4PhantomParameterisation
  G4PhantomParameterisation* thePhantomParam = 0;
  G4AffineTransform theTransformation;
  G4int theNoVoxelsX;
  G4int theNoVoxelsY;
  G4int theNoVoxelsZ;
  std::vector<G4VPhysicalVolume*>::const_iterator ite;
  G4PhysicalVolumeStore* pvstore = G4PhysicalVolumeStore::GetInstance();
  for( ite = pvstore->begin(); ite != pvstore->end(); ite++ ) {
    if ( (*ite)->IsReplicated() )  {
      EAxis axis;
      G4int nReplicas;
      G4double width,offset;
      G4bool consuming;      
      (*ite)->GetReplicationData(axis,nReplicas,width,offset,consuming);
      if( !consuming && (*ite)->GetRegularStructureId() != 0 ) {  
	const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*ite);
	G4VPVParameterisation* param = pvparam->GetParameterisation();
	thePhantomParam = static_cast<G4PhantomParameterisation*>(param);
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels volume found " << (*ite)->GetName() << G4endl;
#endif
	
	//---- Get container transformation
	GmTouchable* containerTouch = geomUtils->GetTouchables( (*ite)->GetMotherLogical()->GetName() )[0];
	theTransformation = G4AffineTransform( containerTouch->GetGlobalRotation(), containerTouch->GetGlobalPosition() );
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels container touchable " << *containerTouch << G4endl;
#endif
	
	G4VPhysicalVolume* containerPV = geomUtils->GetPhysicalVolumes( (*ite)->GetMotherLogical()->GetName() )[0];
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels container physvol " << containerPV->GetTranslation() << G4endl;
#endif
	
	//---- Get voxel dimensions
	theVoxelDimX = thePhantomParam->GetVoxelHalfX();
	theVoxelDimY = thePhantomParam->GetVoxelHalfY();
	theVoxelDimZ = thePhantomParam->GetVoxelHalfZ();
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels voxel dimensions " << theVoxelDimX << " " << theVoxelDimY << " " << theVoxelDimZ << G4endl;
#endif
	theNoVoxelsX = thePhantomParam->GetNoVoxelsX();
	theNoVoxelsY = thePhantomParam->GetNoVoxelsY();
	theNoVoxelsZ = thePhantomParam->GetNoVoxelsZ();
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels number of voxels " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
#endif
	
	break; // do not look for another volume
      }
    }
  }

  if( !thePhantomParam ) {
    G4Exception("GmGenerDistPositionInMaterialPhantomVoxels::SetParams",
		"Wrong distirbution",
		FatalErrorInArgument,
		"No G4PhantomParameterisation exists");
  } 

  // BUILD MATE IDS
  std::vector<G4Material*> ppMates = thePhantomParam->GetMaterials();
  G4cout << thePhantomParam << " thePhantomParam->GetMaterials(); " << ppMates.size() << G4endl;
  for( size_t ii = 0; ii < theMateNames.size(); ii++ ) {
    G4bool bParamFound = false;
    for( size_t iim = 0; iim < ppMates.size(); iim++ ) {
      //      G4cout << ii << " " << iim << " GmGenerDistPositionInMaterialPhantomVoxels mate " << theMateNames[ii] << " " <<  ppMates[iim]->GetName() << G4endl; //GDEB
      if( GmGenUtils::AreWordsEquivalent(theMateNames[ii], ppMates[iim]->GetName()) ) {
	theMateIDs.insert(iim);
	bParamFound = true;
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels MateID included " << ppMates[iim]->GetName() << " from " << theMateNames[ii] << G4endl;
#endif
	//	break;
      }
    }
    if( bParamFound == false ) {
      G4Exception("GmGenerDistPositionInMaterialPhantomVoxels::BuildVoxelPosition",
		  "",
		  FatalErrorInArgument,
		  ("No Material found with name "+theMateNames[ii]).c_str());
    }      
  }

  // build list of voxels satisfying filters 
  // build filters 
  std::vector<GmVFilter*> theFilters;
  std::vector<G4String> filterNames;
  filterNames = GmParameterMgr::GetInstance()->GetVStringValue("GmGenerDistPositionInMaterialPhantomVoxels:Filters",filterNames);
  for( size_t ii = 0; ii < filterNames.size(); ii++ ) {
    std::vector<G4String> params; 
    params.push_back(filterNames[ii]);
    params.push_back(filterNames[ii]);
    GmVFilter* filter = GmFilterMgr::GetInstance()->FindOrBuildFilter(params,true);
    theFilters.push_back(filter);
  }

  //--- Create G4Step
  G4DynamicParticle* DP = new G4DynamicParticle(G4Geantino::Geantino(), G4ThreeVector(1,0,0), 1.);
  G4Track* newTrack = new G4Track( DP, 0., G4ThreeVector(0,0,0));
  newTrack->SetTrackID( 1 );
  G4Step* newStep = new G4Step();
  G4StepPoint* preSP = new G4StepPoint();
  preSP->SetPosition(G4ThreeVector(0,0,0));
  preSP->SetLocalTime(0);
  preSP->SetGlobalTime(0);
  preSP->SetProperTime(0);
  preSP->SetMomentumDirection(G4ThreeVector(1,0,0));
  preSP->SetKineticEnergy(1);
  preSP->SetVelocity(0.1);
  //  preSP->SetTouchableHandle(const G4TouchableHandle& apValue);
  //  preSP->SetMaterial(G4Material*);
  //  preSP->SetMaterialCutsCouple(const G4MaterialCutsCouple*);
  preSP->SetSensitiveDetector((G4VSensitiveDetector*)0);
  preSP->SetSafety(1.);
  preSP->SetPolarization(G4ThreeVector(0,0,0));
  preSP->SetStepStatus(fGeomBoundary);
  preSP->SetProcessDefinedStep((G4VProcess*)0);
  preSP->SetMass(1.);
  preSP->SetCharge(1.);
  preSP->SetMagneticMoment(1.);
  preSP->SetWeight(1.);

  G4StepPoint* postSP = new G4StepPoint();
  postSP->SetPosition(G4ThreeVector(0,0,0));
  postSP->SetLocalTime(0);
  postSP->SetGlobalTime(0);
  postSP->SetProperTime(0);
  postSP->SetMomentumDirection(G4ThreeVector(1,0,0));
  postSP->SetKineticEnergy(1);
  postSP->SetVelocity(0.1);
  //  postSP->SetTouchableHandle(const G4TouchableHandle& apValue);
  //  postSP->SetMaterial(G4Material*);
  //  postSP->SetMaterialCutsCouple(const G4MaterialCutsCouple*);
  postSP->SetSensitiveDetector((G4VSensitiveDetector*)0);
  postSP->SetSafety(1.);
  postSP->SetPolarization(G4ThreeVector(0,0,0));
  postSP->SetStepStatus(fGeomBoundary);
  postSP->SetProcessDefinedStep((G4VProcess*)0);
  postSP->SetMass(1.);
  postSP->SetCharge(1.);
  postSP->SetMagneticMoment(1.);
  postSP->SetWeight(1.);
  newStep->SetPreStepPoint( preSP );
  newStep->SetPostStepPoint( postSP );
  newStep->SetTrack(newTrack);
  newTrack->SetStep(newStep);

  // step length                                                                                        
  newStep->SetStepLength(0.);
  // total energy deposit                                                                                       
  newStep->SetTotalEnergyDeposit(0.);
  // total non-ionizing energy deposit   
  newStep->SetNonIonizingEnergyDeposit(0.);
  // control flag for stepping
  newStep->SetControlFlag(NormalCondition);
  // newStep->SetFirstStepFlag();
  // newStep->SetLastStepFlag();

  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  size_t nvphanX = thePhantomParam->GetNoVoxelsX();
  size_t nvphanY = thePhantomParam->GetNoVoxelsY();
  size_t nvphanXY = nvphanX*nvphanY;
  for( G4int iz = 0; iz < theNoVoxelsZ; iz++ ) { 
    for( G4int iy = 0; iy < theNoVoxelsY; iy++ ) {
      for( G4int ix = 0; ix < theNoVoxelsX; ix++ ) {
	G4int copyNo = ix + nvphanX*iy + nvphanXY*iz;
	// check if voxels is in list of structures
	size_t mateid = thePhantomParam->GetMaterialIndex( ix, iy, iz );    
	G4bool bInMate = false;
	if( theMateIDs.find(mateid) != theMateIDs.end() ) bInMate = true;
	//	G4cout << "GmGenerDistPositionInMaterialPhantomVoxels CHECK INDEX " << copyNo << " mateid " << mateid << " bInMate= " << bInMate << G4endl; //GDEB
	if( !bInMate ) continue;
	
	G4ThreeVector pos = thePhantomParam->GetTranslation( copyNo );
	theTransformation.ApplyPointTransform(pos);	
	G4TouchableHistory* theTouchable = new G4TouchableHistory;
	theNavigator->LocateGlobalPointAndUpdateTouchable( pos, theTouchable, false ); 
	G4Material* material = thePhantomParam->GetMaterial(copyNo); 
	preSP->SetTouchableHandle(theTouchable);
	preSP->SetMaterial(material);  
	//	postSP->SetTouchableHandle(theTouchable);
	//	postSP->SetMaterial(material);  
	//	newTrack->SetTouchableHandle(theTouchable);
	//  postSP->SetMaterialCutsCouple(const G4MaterialCutsCouple*);
	std::vector<GmVFilter*>::const_iterator itef;
	G4bool bPass = true;
	for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ){
	  if( !(*itef)->AcceptStep(newStep) ) {
	    bPass = false;
	    break;
	  }
	}
	if( bPass ) {
	  theVoxelPositions.push_back( pos );
#ifndef GAMOS_NO_VERBOSE
	  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels voxel pass " << copyNo << " pos " << pos << " NVOXELS " << theVoxelPositions.size() << G4endl;
#endif
	}
      }
    }
  }
}


//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionInMaterialPhantomVoxels::GeneratePosition( const GmParticleSource* )
{
  if( !bInitialised ) {
      BuildVoxelPositions();
      bInitialised = true;
  }
  //----- Select randomly a voxel
  G4int voxelID = CLHEP::RandFlat::shoot() * theVoxelPositions.size();
  //  G4cout << "GmGenerDistPositionInMaterialPhantomVoxels " <<  theVoxelPositions.size() << " " << voxelID << G4endl; //GDEB
  G4ThreeVector pos = theVoxelPositions[voxelID]; // voxel centre
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels::GeneratePosition voxelID " << voxelID << " pos " << pos << G4endl;
#endif

  //---- Select randomly a point inside voxel
  pos += G4ThreeVector( -theVoxelDimX + 2*CLHEP::RandFlat::shoot()*theVoxelDimX,
			-theVoxelDimY + 2*CLHEP::RandFlat::shoot()*theVoxelDimY,
			-theVoxelDimZ + 2*CLHEP::RandFlat::shoot()*theVoxelDimZ );
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels::GeneratePosition  pos FINAL " << pos << G4endl;
#endif

  return pos;
}

//---------------------------------------------------------------------
void GmGenerDistPositionInMaterialPhantomVoxels::SetParams( const std::vector<G4String>& params )
{
  if( ReadDICOMVerb(debugVerb) ) G4cout << "GmGenerDistPositionInMaterialPhantomVoxels::SetParams" << G4endl;
  if( params.size() < 1 ) {
    G4Exception(" GmGenerDistPositionInMaterialPhantomVoxels::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"You need a list of structures, there is none");
  }

  G4PhantomParameterisation* thePhantomParam = GmGeometryUtils::GetInstance()->GetPhantomParam(true);
  std::vector<G4Material*> ppMates = thePhantomParam->GetMaterials();

  for( size_t ii = 0; ii < params.size(); ii++ ) {
    theMateNames.push_back(params[ii]);
  }

}

