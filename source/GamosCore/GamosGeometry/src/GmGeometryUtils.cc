#include "GmGeometryUtils.hh"
#include "GmTouchable.hh"
//#include "GmParallelToMassUA.hh"

#include "GmGeomVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Run.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4Track.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4RegionStore.hh"
#include "G4PVParameterised.hh"
#include "G4DensityEffectData.hh"
#include "G4PhantomParameterisation.hh"
#include "CLHEP/Units/SystemOfUnits.h"

#include <iomanip>

GmGeometryUtils* GmGeometryUtils::theInstance = 0;

//-----------------------------------------------------------------------
GmGeometryUtils* GmGeometryUtils::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmGeometryUtils;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmGeometryUtils::GmGeometryUtils() 
{
  theTopPV = 0;

  BuildDictionaries();

  bPreCalculatedPhantomCubicVolumes = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmGeometryUtils:UsePreCalculatePhantomCubicVolumes",0));

  theTouchable = 0;
}
 

//-----------------------------------------------------------------------
GmGeometryUtils::~GmGeometryUtils() 
{
}
 

//-----------------------------------------------------------------------
void GmGeometryUtils::DumpSummary( std::ostream& out )
{

  //---------- Dump number of objects of each class
  out << " @@@@@@@@@@@@@@@@@@ Dumping G4 geometry objects Summary " << G4endl;
  theTopPV = GetTopPV();
  
  if( theTopPV == 0) {
    out << " No volume created " << G4endl;
    return;
  }
  out << " @@@ Geometry built inside world volume: " << theTopPV->GetName() << G4endl;
  //Get number of solids (< # LV if several LV share a solid)
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  std::set<G4VSolid*> theSolids;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    theSolids.insert( (*lvcite)->GetSolid() );
  }
  out << " Number of G4VSolid's: " << theSolids.size() << G4endl;
  out << " Number of G4LogicalVolume's: " << lvs->size() << G4endl;
  const G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  out << " Number of G4VPhysicalVolume's: " << pvs->size() << G4endl;

  //  out << " Number of Touchable's: " << CountNoTouchables() << G4endl;

  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  out << " Number of G4Material's: " << matTab->size() << G4endl;

}

 
/*
//-----------------------------------------------------------------------
G4LogicalVolume* GmGeometryUtils::FindLV( G4String lvname, G4bool mustExist )
{
  G4LogicalVolume* lv = 0;

  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  G4int found = 0;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    if( (*lvcite)->GetName() == lvname ) {
      found++;
      lv = *lvcite;
    }
  }

  if( found == 0 ) {
    if( mustExist ) {
      DumpG4LVTree();
      G4Exception("GmGeometryUtils::FindLV",
      "Wrong argument",
      FatalErrorInArgument,
      G4String("Volume not found " + lvname ).c_str());
    } else {
      G4cerr << " !! WARNING at GmGeometryUtils::FindLV: volume not found with name" << lvname << G4endl;
    }
  } else if( found > 1 ) {
      DumpG4LVTree();
      G4cerr << " !! SEVERE WARNING at GmGeometryUtils::FindLV: more than 1 logical volume found with name " << lvname << G4endl;
  }

  return lv;

}
*/

//-----------------------------------------------------------------------
void GmGeometryUtils::DumpG4LVList( std::ostream& out )
{
  out << " @@@@@@@@@@@@@@@@ DUMPING G4LogicalVolume's List  " << G4endl;
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    out << "LV: " << (*lvcite)->GetName() << G4endl;
  }

}


//-----------------------------------------------------------------------
void GmGeometryUtils::DumpG4LVTree( std::ostream& out )
{
  out << " @@@@@@@@@@@@@@@@ DUMPING G4LogicalVolume's Tree  " << G4endl;
  G4LogicalVolume* lv = GetTopLV();
  
  DumpG4LVLeaf( lv, 0, out );
}

//-----------------------------------------------------------------------
G4Material* GmGeometryUtils::GetMaterial( const G4String& name, bool exists )
{
  G4Material* mate = 0;
  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  std::vector<G4Material*>::const_iterator matite;
  for( matite = matTab->begin(); matite != matTab->end(); matite++ ) {
    if( GmGenUtils::AreWordsEquivalent( name, (*matite)->GetName()) ) {
      //    if( (*matite)->GetName() == name ) {
      if( mate != 0 ) {
	G4Exception("GmGeometryUtils::GetMaterial","WARNING",JustWarning,("More than one material with same name: "+name + " The first will be taken").c_str());
      }
      mate = *matite;
    }
  }

  if( mate == 0 ){
    if( exists ) {
      G4Exception("GmGeometryUtils::GetMaterial","ERROR",FatalErrorInArgument,("Material does not exist: " + name).c_str());
    }

  }
  return mate;
}

//-----------------------------------------------------------------------
std::vector<G4Material*> GmGeometryUtils::GetAllMaterials( const G4String& name, bool exists )
{
  std::vector<G4Material*> mates;
  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  std::vector<G4Material*>::const_iterator matite;
  for( matite = matTab->begin(); matite != matTab->end(); matite++ ) {
    if( GmGenUtils::AreWordsEquivalent( name, (*matite)->GetName()) ) {
      mates.push_back(*matite);
    }
  }

  if( mates.size() == 0 ){
    if( exists ) {
      G4Exception("GmGeometryUtils::GetMaterial","ERROR",FatalErrorInArgument,("Material does not exist: " + name).c_str());
    }

  }
  return mates;
}

//-----------------------------------------------------------------------
std::vector<G4Material*> GmGeometryUtils::GetMaterials( const G4String& name, bool exists )
{
  std::vector<G4Material*> mates;
  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  std::vector<G4Material*>::const_iterator matite;
  for( matite = matTab->begin(); matite != matTab->end(); matite++ ) {
    if( GmGenUtils::AreWordsEquivalent( name, (*matite)->GetName() ) ) {
      //    if( (*matite)->GetName() == name ) {
      mates.push_back(*matite);
    }
  }

  if( mates.size() == 0 ){
    if( exists ) {
      G4Exception("GmGeometryUtils::GetMaterials","ERROR",FatalErrorInArgument,("Material does not exist: " + name).c_str());
    } else {
      G4Exception("GmGeometryUtils::GetMaterials","WARNING",JustWarning,("Material does not exist: " + name).c_str());
    }

  }
  return mates;
}


//-----------------------------------------------------------------------
void GmGeometryUtils::DumpMaterialList( std::ostream& out, G4int verbosity )
{
  out << " @@@@@@@@@@@@@@@@ DUMPING G4Material List ";
  const G4MaterialTable* matTab = G4Material::GetMaterialTable();
  out << " with " << matTab->size() << " materials " << G4endl;
  std::vector<G4Material*>::const_iterator matite;
  for( matite = matTab->begin(); matite != matTab->end(); matite++ ) {
    G4Material* material = *matite;
    out << "Material: " << material << G4endl;
    if( verbosity >= 1) {
      const G4ElementVector* elemV = material->GetElementVector();
      const G4double* elemFracV = material->GetFractionVector();
      for( unsigned int ie = 0; ie < elemV->size(); ie++ ){
	G4Element* element = (*elemV)[ie];
	G4String elemName = element->GetName();
	G4cout << "  Element: " << elemName 
	       << " " << elemFracV[ie] << G4endl;
	// ISOTOPES                                                               
	G4IsotopeVector* isotV = element->GetIsotopeVector();
	G4double* isotFracV = element->GetRelativeAbundanceVector();
	for( unsigned int ip = 0; ip < isotV->size(); ip++ ){
	  G4Isotope* isotope = (*isotV)[ip];
	  G4String isotName = isotope->GetName();
	  G4cout << "    Isotope: " << isotName << " " << elemFracV[ie]*isotFracV[ip] << "  " << isotFracV[ip] << G4endl;
 
	}
      }
    }
    
    if( verbosity >= 2) {
      G4IonisParamMat::GetDensityEffectData()->PrintData(material->GetName()); 
    }
    
  }

}


//---------------------------------------------------------------------------
void GmGeometryUtils::DumpG4LVLeaf( G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out )
{
  unsigned int ii;
  for( ii=0; ii < leafDepth; ii++ ){
    out << "  ";
  }
  out << " LV:(" << leafDepth << ") " << lv->GetName() << G4endl;
  unsigned int siz = lv->GetNoDaughters();
  //--- If a volume is placed n types as daughter of this LV, it should only be counted once
  std::set< G4LogicalVolume* > lvDaughters;
  for( ii = 0; ii < siz; ii++ ){
    lvDaughters.insert( lv->GetDaughter(ii)->GetLogicalVolume() );
  }

  std::set< G4LogicalVolume* >::const_iterator cite;
  for(cite = lvDaughters.begin(); cite != lvDaughters.end(); cite++ ){
    DumpG4LVLeaf( *cite, leafDepth+1, out );
  } 
}


//-----------------------------------------------------------------------
int GmGeometryUtils::CountNoTouchables()
{
  //t
  return 0;

  int nTouch = 0;
  G4LogicalVolume* lv = GetTopLV();
  
  Add1Touchable( lv, nTouch, 1 );
  return nTouch;
}


//---------------------------------------------------------------------------
void GmGeometryUtils::Add1Touchable( G4LogicalVolume* lv, int& nTouch, int nReplicas )
{
  int siz = lv->GetNoDaughters();
  for( int ii = 0; ii < siz; ii++ ){

    G4int newnReplicas = 1;
    if( lv->GetDaughter(ii )->IsReplicated() ) {
      EAxis axis;
      G4double width;
      G4double offset;
      G4bool consuming;
      lv->GetDaughter(ii )->GetReplicationData(axis, newnReplicas, width, offset, consuming );
    }

    nReplicas *= newnReplicas;
    nTouch += nReplicas;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " Add1Touchable " << nTouch << " nReplicas " << nReplicas << " newnReplicas " << newnReplicas << " " << lv->GetDaughter(ii )->GetName() << G4endl;
#endif
    Add1Touchable( lv->GetDaughter(ii )->GetLogicalVolume(), nTouch, nReplicas );
  } 
}
 

//-----------------------------------------------------------------------
void GmGeometryUtils::DumpG4PVLVTree( std::ostream& out, G4int verbosity ) 
{
  //dumps in the following order: 
  //    1) a LV with details
  //    2) list of PVs daughters of this LV with details
  //    3) list of LVs daughters of this LV and for each go to 1)

  //----- Get top PV
  G4LogicalVolume* topLV = GetTopLV();
  
  G4cout << " DumpG4PVLVTree " << verbosity << G4endl;
  //----- Dump this leave (it will recursively dump all the tree)
  DumpPV( theTopPV, 0, out, verbosity );
  DumpG4PVLVLeaf( topLV, 0, out, verbosity );
  
  //----- Dump the touchables (it will recursively dump all the tree)
//t  DumpTouch( theTopPV, 0, out);
}


//-----------------------------------------------------------------------
void GmGeometryUtils::DumpG4PVLVLeaf(G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out, G4int verbosity)
{
  //-  leafDepth++;
 //----- Dump this LV 
  DumpLV( lv, leafDepth, out, verbosity );

  //----- Get LV daughters from list of PV daughters
  mmlvpv lvpvDaughters;
  std::set< G4LogicalVolume* > lvDaughters;
  int NoDaughters = lv->GetNoDaughters();
  while((NoDaughters--)>0){
    G4VPhysicalVolume* pvD = lv->GetDaughter(NoDaughters);
    lvpvDaughters.insert( mmlvpv::value_type( pvD->GetLogicalVolume(), pvD ) );
    lvDaughters.insert( pvD->GetLogicalVolume() );
  }
 
  std::set< G4LogicalVolume* >::const_iterator scite;
  mmlvpv::const_iterator mmcite;

   //----- Dump daughters PV and LV
  for( scite = lvDaughters.begin(); scite != lvDaughters.end(); scite++ ) {
    std::pair< mmlvpv::iterator, mmlvpv::iterator > mmER = lvpvDaughters.equal_range( *scite );
    
   //----- Dump daughters PV of this LV
    for( mmcite = mmER.first ; mmcite != mmER.second; mmcite++ ) {
      DumpPV( (*mmcite).second, leafDepth+1, out, verbosity );
    }
    //    G4LogicalVolume *DaughterLogVol; 
   //----- Dump daughters LV
    DumpG4PVLVLeaf( *scite, leafDepth+1, out, verbosity );
  }
}
 
//-----------------------------------------------------------------------
void GmGeometryUtils::DumpLV(G4LogicalVolume* lv, unsigned int leafDepth, std::ostream& out, G4int verbosity)
{
  int dumpVerbose = (verbosity)%10;
  G4String spaces = SpacesFromLeafDepth( leafDepth );

  //----- dump name 
  if( dumpVerbose >= 1) {
    out << leafDepth << spaces << "$$$ LOGICAL VOLUME = " << lv->GetName();
    if( dumpVerbose == 1 ) out << G4endl;
  }
  if( dumpVerbose >= 2) {
    out << "  Solid: " << lv->GetSolid()->GetName() 
	<< "  MATERIAL: " << lv->GetMaterial()->GetName()
	<< "  CUBIC_VOLUME: " << lv->GetSolid()->GetCubicVolume()/CLHEP::gram 
	<< "  MASS: " << lv->GetMass()/CLHEP::gram << G4endl;
  } else if (dumpVerbose == 0) {
    out << G4endl;
  }
  if( dumpVerbose >= 3) {
    //----- dump solid
    DumpSolid( lv->GetSolid(), leafDepth, out );
  }

  //----- dump LV info 
  //--- material 
  if (dumpVerbose >= 3 ) {
    //--- Visualisation attributes
    const G4VisAttributes* fVA = lv->GetVisAttributes();
    if( fVA ) { 
      out << spaces << "  VISUALISATION ATTRIBUTES: " << G4endl;
      out <<  spaces << "    IsVisible " << fVA->IsVisible() << G4endl;
      out <<  spaces << "    IsDaughtersInvisible " << fVA->IsDaughtersInvisible() << G4endl;
      out <<  spaces << "    Colour " << fVA->GetColour() << G4endl;
      out <<  spaces << "    LineStyle " << fVA->GetLineStyle() << G4endl;
      out <<  spaces << "    LineWidth " << fVA->GetLineWidth() << G4endl;
      out <<  spaces << "    IsForceDrawingStyle " << fVA->IsForceDrawingStyle() << G4endl;
      out <<  spaces << "    ForcedDrawingStyle " << fVA->GetForcedDrawingStyle() << G4endl;
    }    
    
    //--- User Limits
    G4UserLimits *fUL = lv->GetUserLimits();
    G4Track dummy;
    if( fUL ) {
      out <<  spaces << "    MaxAllowedStep " << fUL->GetMaxAllowedStep(dummy) << G4endl;
      out <<  spaces << "    UserMaxTrackLength " << fUL->GetUserMaxTrackLength(dummy) << G4endl;
      out <<  spaces << "    UserMaxTime " << fUL->GetUserMaxTime(dummy) << G4endl;
      out <<  spaces << "    UserMinEkine " << fUL->GetUserMinEkine(dummy) << G4endl;
      out <<  spaces << "    UserMinRange " << fUL->GetUserMinRange(dummy) << G4endl;
    }
    
    //--- other LV info
    if( lv->GetSensitiveDetector() ) {
      out << spaces << "  IS SENSITIVE DETECTOR " << G4endl;
    }
    if( lv->GetFieldManager() ) {
      out << spaces << "  FIELD ON " << G4endl;
    }
    // G4SmartVoxelHeader *fVoxel;
    // Pointer (possibly NULL) to optimisation info objects.
    out <<  spaces << "        Quality for optimisation, average number of voxels to be spent per content " << lv->GetSmartless() << G4endl;
    //    G4FastSimulationManager *fFastSimulationManager;
    // Pointer (possibly NULL) to G4FastSimulationManager object.
    if( lv->GetFastSimulationManager() ) {
      out << spaces << "     Logical Volume is an envelope for a FastSimulationManager " << G4endl;
    }
    out << spaces << "     Weight used in the event biasing technique = " << lv->GetBiasWeight() << G4endl;
  }
  
}


//-----------------------------------------------------------------------
void GmGeometryUtils::DumpPV(G4VPhysicalVolume* pv, unsigned int leafDepth, std::ostream& out, G4int verbosity)
{
  G4cout << " DumpPV " << verbosity << G4endl;

  if( pv == 0 ) return;
  int dumpVerbose = (verbosity/10)%10;
  G4String spaces = SpacesFromLeafDepth( leafDepth );

  //----- PV info
  if( dumpVerbose >= 1) {
    G4String mother = "World";
    if (pv->GetMotherLogical()) mother = pv->GetMotherLogical()->GetName();
    out << leafDepth << spaces << "### PHYSICAL VOLUME = " << pv->GetName() 
	<< " Copy No " << pv->GetCopyNo()
	<< " in " << mother;
    if( dumpVerbose == 1) out << G4endl;
  }
  
  if( dumpVerbose >= 2) {
    out << std::setprecision(8) << " at " << pv->GetTranslation();
  }

  if( !pv->IsReplicated() ) {
    if( pv->GetRotation() == 0 ) {
      if( dumpVerbose >= 2) {
	out << " with no rotation" << G4endl;
      }
    } else {
      if( dumpVerbose >= 2 ){
	out  << " with rotation " << *(pv->GetRotation()) << G4endl;
      }
    }
  } else {
    if( dumpVerbose >= 3 ){
      out << spaces << "    It is replica: " << G4endl;
      EAxis axis; G4int nReplicas; G4double width; G4double offset; G4bool consuming;
      pv->GetReplicationData( axis, nReplicas, width, offset, consuming);
      out << spaces << "     axis " << axis << G4endl
	  << spaces << "     nReplicas " << nReplicas << G4endl;
      if( pv->GetParameterisation() != 0 ) {
	out << spaces << "    It is parameterisation " << G4endl;
      } else { 
	out << spaces << "     width " << width << G4endl
	    << spaces << "     offset " << offset << G4endl
	    << spaces << "     consuming" <<  consuming << G4endl;
      }
      if( pv->GetParameterisation() != 0 ) {
	out << spaces << "    It is parameterisation " << G4endl;
      }
      
    }
  }
}


/*//-----------------------------------------------------------------------
void GmGeometryUtils::DumpTouch(G4VPhysicalVolume* pv, unsigned int leafDepth, std::ostream& out)
{
  int dumpVerbose = (verbosity/10)%10;
  G4String spaces = SpacesFromLeafDepth( leafDepth );
  if (leafDepth == 0) {
    fHistory.SetFirstEntry(pv);
  } else {
    fHistory.NewLevel(pv, kNormal, pv->GetCopyNo());
  }

  G4ThreeVector globalpoint = fHistory.GetTopTransform().Inverse().
    TransformPoint(G4ThreeVector(0,0,0));
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  if ( dumpVerbose > 0) {
    G4String mother = "World";
    if (pv->GetMotherLogical()) mother = pv->GetMotherLogical()->GetName();
    G4String lvname = lv->GetName();
    lvname.assign(lvname,0,nchar);
    if (lvname == name)
      out << leafDepth << spaces << "### VOLUME = " << lv->GetName() 
	  << " Copy No " << pv->GetCopyNo() << " in " << mother
	  << " global position of centre " << globalpoint << G4endl;
  }

  int NoDaughters = lv->GetNoDaughters();
  while((NoDaughters--)>0) {
    G4VPhysicalVolume* pvD = lv->GetDaughter(NoDaughters);
    if ( !pvD->IsReplicated() ) {
    //  DumpTouch( pvD, leafDepth+1, out  );
    }
  }

  if (leafDepth > 0) fHistory.BackLevel();
}
*/

 //-----------------------------------------------------------------------
G4String GmGeometryUtils::SpacesFromLeafDepth( unsigned int leafDepth )
{
  G4String spaces;
  unsigned int ii;
  for( ii = 0; ii < leafDepth; ii++ ){
    spaces += "  ";
  }
  return spaces;
}


//-----------------------------------------------------------------------
void GmGeometryUtils::DumpSolid( G4VSolid* sol, unsigned int leafDepth, std::ostream& out )
{
  G4String spaces = SpacesFromLeafDepth( leafDepth );
  out << spaces << *(sol) << G4endl;
}


//-----------------------------------------------------------------------
G4VPhysicalVolume* GmGeometryUtils::GetTopPV()
{
 return G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();
}


//-----------------------------------------------------------------------
G4LogicalVolume* GmGeometryUtils::GetTopLV(G4bool bGeomInit)
{
  G4LogicalVolume* topLV ;
  theTopPV = GetTopPV();
  if( bGeomInit && theTopPV ) {
    topLV = theTopPV->GetLogicalVolume();
  } else {
    G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
    std::vector<G4VPhysicalVolume*>::iterator cite;
    for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
      if( (*cite)->GetMotherLogical() == 0 ) break;
    }
    topLV = (*cite)->GetLogicalVolume();
  }

  return topLV;

}


//-----------------------------------------------------------------------
std::vector<GmTouchable*> GmGeometryUtils::GetTouchables( const G4String& name, bool exists )
{
  theAncestorLines.clear();
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetTouchables " << name << G4endl;
#endif

  //---------- clean theAncestorLines calculated previously
  theAncestorLines.clear();

  //---------- 'ancestorsRequested' will host the list of ancestors in pairs ancestor name - ancestor copy number 
  vpsi ancestorsRequested = ExtractAncestorsRequested( name );

  //---------- Name of volume requested is the last name in the list of ancestors
  G4String volName = ancestorsRequested[0].first;

  //--------- Get how many physical volumes are there of each ancestor: if there are 4 volumes A, each volume A is placed 2 times in volume B and volume B 3 times in volume C, there has to be constructed 4 X 2 X 3 GmTouchable's

  // loop to all the PV's with the same name than volName
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
  //----- For each PV: 
  //--(assume PV is inside LV)
  //--- 1. get the LV corresponding to this PV
  //--- 2. get the LV parent
  //--- 3. get all PV corresponding to this LV
    G4VPhysicalVolume* pv0 = *cite;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << "@@@ GmGeometryUtils::GetTouchables: Looking for: " << pv0->GetName() << G4endl;
#endif
    if( GmGenUtils::AreWordsEquivalent( volName, pv0->GetName() ) ) {
      //    if( (*cite)->GetName() == name ) {
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetTouchables FOUND: " << pv0->GetName() << " :" << pv0->GetCopyNo() << G4endl;
#endif
      //--- create vector of PV ancestors of this PV: each PV will have associated a vector of its PV's ancestors. All the vectors will be then stored in 'theAncestorLines'
      std::vector< G4VPhysicalVolume* > vpv; // vector of PV's ancestors

      //(( the following 3 lines should be moved to AddParentPV (where a PV register itself and then the parents) ))
      //--- check that it is the copy number we are looking for
      bool copyOK = CheckPVCopyNo( pv0, ancestorsRequested[0].second );
      if( copyOK ) { 
	//-- add it to the vector of ancestors as the first one
	vpv.push_back( pv0 );
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) G4cout << " GmGeometryUtils::GetTouchables Added to PV's ancestor list: " << pv0->GetName() << " N=" << vpv.size() << G4endl;
#endif
	//-- add parent (recursively all ancestors will be added)
	AddParentPV( pv0, vpv, ancestorsRequested, 0);
      }
    }
  }
  
  //----- Construct GmTouchable's for each line of PV's ancestors 
  std::vector<GmTouchable*> vtouch;
  G4int jj, kk;
  G4int asiz = theAncestorLines.size();

  //--- Loop to each line of PV's ancestors
  for( G4int ii = 0; ii < asiz; ii++ ) {
    std::vector<G4VPhysicalVolume*> vpv = theAncestorLines[ii];
    G4int siz =  vpv.size();
    std::vector<G4int> vi;
    //--- build list of copyNo's of each ancestor:
    // If it is replica of parameterisation 1 PV has many touchables. Therefore if copyNo is -1, this means that you want all the touchables of the replica, so you have to expand it
    for( jj = 0; jj < siz; jj++ ){
      G4int copyn;
      G4int copyNoAncestor;
      if( !vpv[jj]->IsReplicated() ) {
	//-- Simple placement: the copyNo you are requesting is the PV's copyNo (already checked in AddParentPV method)
	copyNoAncestor = vpv[jj]->GetCopyNo(); 
	//-	cout << "test " << vpv[jj]->GetCopyNo() << " != " << ancestorsRequested[jj].second << G4endl;
	//-	if (vpv[jj]->GetCopyNo() != ancestorsRequested[jj].second ) exit(1);
      } else {
	//-- replica/parameterisation: PV::copyNo has no sense, requested copyNo is actually meaning the number of the G4Touchable, not the G4PhysicalVolume
	if( jj < G4int(ancestorsRequested.size()) ) {
	  copyNoAncestor = ancestorsRequested[jj].second;
	} else { 
	  copyNoAncestor = -1;
	}
      }

      //here -1 means that it is a replica and you have not requested any copy number of it. If it were a simple placement and you have not requested any copy number, then you would have build an ancestor line for each PV
      if( copyNoAncestor == -1 ) {
	//-	if ( !vpv[jj]->IsReplicated() ) exit(1);
	EAxis axis;
	G4int nReplicas;
	G4double width;
	G4double offset;
	G4bool consuming;
	vpv[jj]->GetReplicationData(axis, nReplicas, width, offset, consuming);
	//--- we set copyn so that it will be G4interpreted in method ExpandCopyNolist
	copyn = -10-nReplicas; 
      } else {
	copyn =  copyNoAncestor;
      }

      vi.push_back( copyn );
    }

    //----- Now expand list of copyNo's, if needed
    std::vector< std::vector<G4int> > vvi;
    for( kk = 0; kk < G4int(vi.size()); kk++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << kk << " calling ExpandCopyNoList " << vi[kk] << G4endl;
#endif
    }
    ExpandCopyNoList( vvi, vi, 0 );

    //----- Loop to the expanded list of copyNo's of this ancestor line
    G4int sizv = vvi.size();
    //t G4cout << ii <<  " panded list size " << sizv << G4endl;

    for( kk = 0; kk < sizv; kk++ ) {
    //----- dump ancestor line
      if( GeomVerb(debugVerb) ) {
       	G4cout << ii << " theAncestorLine: ";
	for( jj = 0; jj < G4int(vpv.size()); jj++ ){
	  G4cout << " " << vpv[jj]->GetName()  << " " << vpv[jj]->GetCopyNo()<< "=" << vvi[kk][jj];
	}
	G4cout << G4endl;
      }

      //----- Build GmTouchable: from vpv you will get the PV info, the list of copyNo's used will be vvi[kk], not the copyNo's of the PV's, because the copy No has no meaning for replicas
      //build the long name to find if it exists already:
      G4String longName = "";
      G4String ancestorName;
      G4int nAnces = vpv.size();
      for( G4int ii2 = 0; ii2 < nAnces-1; ii2++ ) {
	G4VPhysicalVolume* pv = vpv[ii2];
	ancestorName = "/" + pv->GetName();
	char chartmp[10];
	G4int copyNoAncestor;
	if( !pv->IsReplicated() ) {
	  copyNoAncestor = pv->GetCopyNo();
	} else {
	  copyNoAncestor = vvi[kk][ii2];
	}
	copyNoAncestor = vvi[kk][ii2];
	gcvt( copyNoAncestor, 10, chartmp );
	ancestorName += G4String(":") + G4String(chartmp);
	longName =  ancestorName + longName;
      }

      GmTouchable* to = new GmTouchable( vpv, vvi[kk] );

#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << longName << " touchable name " << to->GetLongName() << G4endl;
#endif

      //-      G4cout << " touchable copy N " << to->getCopyNo() << G4endl;

      //----- Add it to the list of touchables to return
      vtouch.push_back( to );
    }
  }

  //----- Check that at least one touchable was found
  if( vtouch.size() == 0 ) {
    if(exists) {
      G4Exception(" GmGeometryUtils::GetTouchables",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("No touchable found with name " + name).c_str());
    } else {
      G4cerr << "!!WARNING: GmGeometryUtils::GetTouchables: no touchable found with name " << name << G4endl;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " vtouch size " << vtouch.size() << G4endl;
#endif
  return vtouch;

}


//-----------------------------------------------------------------------
std::vector<G4VPhysicalVolume*> GmGeometryUtils::GetPhysicalVolumes( const G4String& name, bool exists, G4int nVols )
{
  std::vector<G4VPhysicalVolume*> vvolu;
  std::string::size_type ial = name.rfind(":");
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes volname found ':' at " << ial << " " << name << G4endl;
#endif
  G4String volname = "";
  G4int volcopy = 0;
  if( ial != std::string::npos ) {
    std::string::size_type ial2 = name.rfind(":",ial-1);
    if( ial2 != std::string::npos ) {
      G4Exception("GmGeometryUtils::GetPhysicalVolumes",
      "Wrong argument",
		  FatalErrorInArgument,
		  G4String("ame corresponds to a touchable " + name).c_str());
    }else { 
      volname = name.substr( 0, ial );
      volcopy = GmGenUtils::GetInteger( name.substr( ial+1, name.length() ) );
    }
  } else {
    volname = name;
    volcopy = -1;
  }

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator citepv;
  for( citepv = pvs->begin(); citepv != pvs->end(); citepv++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes  volname " << volname << " copy " << volcopy << " TRY " << (*citepv)->GetName() << " copyN " << (*citepv)->GetCopyNo() << 
	" OK? " <<   GmGenUtils::AreWordsEquivalent( volname, (*citepv)->GetName() )  << G4endl;
#endif
    if( GmGenUtils::AreWordsEquivalent( volname, (*citepv)->GetName() ) 
	&& ( (*citepv)->GetCopyNo() == volcopy || -1 == volcopy ) ){
      vvolu.push_back( *citepv );
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes  volname found N= " <<  vvolu.size() << G4endl;
#endif
    }
  }

  //----- Check that at least one volume was found
  if( vvolu.size() == 0 ) {
    if(exists) {
      DumpG4PVLVTree(G4cout, 1); 
      G4Exception(" GmGeometryUtils::GetPhysicalVolumes",
      "Wrong argument",
      FatalErrorInArgument,
		  G4String("No physical volume found with name " + name).c_str());
    } else {
      G4cerr << "!!WARNING: GmGeometryUtils::GetPhysicalVolumes: no physical volume found with name " << name << G4endl;
    }
  }

  if( nVols != -1 && G4int(vvolu.size()) != nVols ) {
    G4Exception("GmGeometryUtils::GetLogicalVolumes:",
		"Wrong number of physical volumes found",
		FatalErrorInArgument,
		("Number of physical volumes " + GmGenUtils::itoa(vvolu.size()) + ", requesting " + GmGenUtils::itoa(nVols)).c_str());
  } 

  return vvolu;
}


//-----------------------------------------------------------------------
std::vector<G4String> GmGeometryUtils::GetPhysicalVolumeNames( const G4String& name, bool exists )
{
  std::vector<G4String> vvolu;
  std::string::size_type ial = name.rfind(":");
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes volname " << ial << " " << name << G4endl;
#endif
  G4String volname = "";
  G4int volcopy = 0;
  if( ial != std::string::npos ) {
    std::string::size_type ial2 = name.rfind(":",ial-1);
    if( ial2 != std::string::npos ) {
      G4Exception("GmGeometryUtils::GetPhysicalVolumes",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Name corresponds to a touchable " + name).c_str());
    }else { 
      volname = name.substr( 0, ial );
      volcopy = GmGenUtils::GetInteger( name.substr( ial+1, name.length() ) );
    }
  } else {
    volname = name;
    volcopy = -1;
  }

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator citepv;
  for( citepv = pvs->begin(); citepv != pvs->end(); citepv++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes  volname " << volname << " copy " << volcopy << " TRY " << (*citepv)->GetName() << " " << (*citepv)->GetCopyNo() << 
	" " <<   GmGenUtils::AreWordsEquivalent( volname, (*citepv)->GetName() )  << G4endl;
#endif
    if( GmGenUtils::AreWordsEquivalent( volname, (*citepv)->GetName() ) 
	&& ( (*citepv)->GetCopyNo() == volcopy || -1 == volcopy ) ){
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::GetPhysicalVolumes  volname found " << G4endl;
#endif
      G4String pvName = (*citepv)->GetName() + ":";
      if( (*citepv)->IsReplicated() ) {
	EAxis axis;
	G4int nReplicas;
	G4double width;
	G4double offset;
	G4bool consuming;
	(*citepv)->GetReplicationData(axis, nReplicas, width, offset, consuming );
	for( G4int ii = 0; ii < nReplicas; ii++ ) {
	  vvolu.push_back( pvName + GmGenUtils::ftoa(ii) );
	}
      } else {
	vvolu.push_back( pvName + GmGenUtils::ftoa((*citepv)->GetCopyNo()) );
      }
    }
  }

  //----- Check that at least one volume was found
  if( vvolu.size() == 0 ) {
    if(exists) {
      DumpG4PVLVTree(G4cout, 1); 
      G4Exception(" GmGeometryUtils::GetPhysicalVolumes",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("No physical volume found with name " + name).c_str());
    } else {
      G4Exception("GmGeometryUtils::GetLogicalVolumes:","ERROR",JustWarning,("GmGeometryUtils::GetPhysicalVolumes: no physical volume found with name " + name).c_str());
    }
  }

  return vvolu;
}


//-----------------------------------------------------------------------
std::vector<G4LogicalVolume*> GmGeometryUtils::GetLogicalVolumes( const G4String& name, bool exists, G4int nVols )
{
  //  G4cout << "GetLogicalVolumes " << name << " " << exists << G4endl;
  std::vector<G4LogicalVolume*> vvolu;
  G4int ial = name.rfind(":");
  if( ial != -1 ) {
    G4Exception("GmGeometryUtils::GetLogicalVolumes",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Name corresponds to a touchable or physcal volume" + name).c_str());
  }

  G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::iterator citelv;
  for( citelv = lvs->begin(); citelv != lvs->end(); citelv++ ) {
    //    if( name == (*citelv)->GetName() ) {
    if( GmGenUtils::AreWordsEquivalent(name,(*citelv)->GetName()) ) {
      //      G4cout << " GmGeometryUtils::GetLogicalVolumes vol found " << (*citelv)->GetName() << " looking for " << name << G4endl;
      vvolu.push_back( *citelv );
    }
  }

  //----- Check that at least one volume was found
  if( vvolu.size() == 0 ) {
    DumpG4LVTree();
    if(exists) {
      G4Exception("GmGeometryUtils::GetLogicalVolumes:","ERROR",FatalErrorInArgument,("no logical volume found with name " + name).c_str());
    } else {
      G4Exception("GmGeometryUtils::GetLogicalVolumes:","ERROR",JustWarning,("no  logical volume found with name " + name).c_str());
    }
  }

  if( nVols != -1 && G4int(vvolu.size()) != nVols ) {
    G4Exception("GmGeometryUtils::GetLogicalVolumes:",
		"Wrong number of logical volumes found",
		FatalErrorInArgument,
		("Number of logical volumes " + GmGenUtils::itoa(vvolu.size()) + ", requesting " + GmGenUtils::itoa(nVols)).c_str());
  } 

  return vvolu;
    
}

//-----------------------------------------------------------------------
std::vector<G4Region*> GmGeometryUtils::GetRegions( const G4String& name, bool exists )
{

  std::vector<G4Region*> regions;
  G4RegionStore* regionStore = G4RegionStore::GetInstance();  
  for( unsigned int jj = 0; jj < regionStore->size(); jj++ ){
    G4Region* reg = (*regionStore)[jj];
    if( GmGenUtils::AreWordsEquivalent(name,reg->GetName()) ){
      regions.push_back(reg);
    }
  }
  
  //----- Check that at least one volume was found
  if( regions.size() == 0 ) {
    if(exists) {
      G4Exception("GmGeometryUtils:::GetRegions","ERROR",FatalErrorInArgument,("no region found with name " + name).c_str());
    } else {
      G4Exception("GmGeometryUtils::GetRegions:","ERROR",JustWarning,("no regionfound with name " + name).c_str());
    }
  }

  return regions;
}

//-----------------------------------------------------------------------
vpsi GmGeometryUtils::ExtractAncestorsRequested( const G4String& name )
{
  //---------- decompose name in the volume's and copyNo's of the ancestors:
  // /volA:2/volB:4/volC:3  =>  <volA,2>,  <volB,4>, <volC,3>
  G4String namet = name;
  std::vector< std::pair<G4String, G4int> > ancestorsRequested;
  //---------- loop until a '/' is found in position 0, or not found
  for(;;) {  
    G4String shortName;
    G4int fnumb = namet.rfind(':');
    G4int fslash = namet.rfind('/');

    //       G4cout << namet << " test fnumb " << fnumb << " fslash " << fslash << G4endl;
    G4int numb;
    if( fnumb < fslash || fnumb == -1) {
      //----- name without number, then set number is -1
      shortName = namet.substr(fslash+1, namet.size()-fslash);
      numb = -1;
    } else {
      shortName = namet.substr(fslash+1, fnumb-fslash-1);
      numb = atoi( (namet.substr(fnumb+1, namet.size()-1)).c_str() );
      //---- check that copy number requested is not negative (or 0)
      if( numb < -1 ) {
	G4cerr << "!!EXITING: GmGeometryUtils::ExtractAncestorsRequested: you cannot ask for a copy number smaller than : 0 " << name << G4endl;
	exit(1);
      }
    }
    //    G4int tt =  atoi( (namet.substr(fnumb+1, namet.size()-1).c_str()) );
    std::pair<G4String, G4int> psi( shortName, numb);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(testVerb) ) G4cout << " GmGeometryUtils::ExtractAncestorsRequested new node " << psi.first << " : " << psi.second << G4endl;
#endif
    ancestorsRequested.push_back( psi );
    if( fslash == 0 || fslash == -1 ) break;
    namet = namet.substr(0, fslash);
  }

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) {
    G4cout << " GmGeometryUtils::ExtractAncestorsRequested from " << name << " : " << std::endl;
    for( size_t ii = 0; ii < ancestorsRequested.size(); ii++ ) {
      std::cout << "    ances: " << ancestorsRequested[ii].first << " copyNo " << ancestorsRequested[ii].second << std::endl;
    }
  }
#endif

  return ancestorsRequested;
}


//-----------------------------------------------------------------------
void GmGeometryUtils::AddParentPV( const G4VPhysicalVolume* pv, std::vector<G4VPhysicalVolume*>& vpv, vpsi& ancestorsRequested, G4int ancestorLevel ) 
{
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "  GmGeometryUtils::AddParentPV of " << pv->GetName() << " :" << pv->GetCopyNo() << " ancestorsRequested.size() " << ancestorsRequested.size() << " ancestorLevel " << ancestorLevel << G4endl;
#endif
  
  //----- 1. get the LV parent:
  // As a PV does not have any pointer to its parent (only if the placement is done inside PV, instead of inside LV, this last being the only option currently supported),  we have to look for its parent LV and then get the list of all the PV's of this LV.
  const G4LogicalVolume* lvParent = thePVLVTree[ const_cast<G4VPhysicalVolume*>(pv) ];

  if( lvParent != 0 ) {
    //----- get which is the parent name and copy number you are looking for 
    G4String parentName;
    G4int parentCopyNo;
    if( ancestorLevel+1 < int(ancestorsRequested.size()) ) {
      parentName = ancestorsRequested[ancestorLevel+1].first;
      parentCopyNo = ancestorsRequested[ancestorLevel+1].second;
    } else {
      //no parent defined, all will serve
      parentName = " ";
      parentCopyNo = -1;
    }

    //----- check that parent is the one asked for
    if( !GmGenUtils::AreWordsEquivalent(parentName, lvParent->GetName())&& parentName != " ") return;
    //    if( lvParent->GetName() != parentName && parentName != " ") return;

    //----- 2.  get all PV corresponding to this LV 
    mmlvpv::const_iterator mmcite;
    std::pair<mmlvpv::iterator, mmlvpv::iterator> mmdi;
    mmdi = theLVPVTree.equal_range( const_cast<G4LogicalVolume*>(lvParent) );
    //    G4cout << " theLVPVTree.equal_range " << ( (*(mmdi.first)).first )->GetName() 
    //	   << " " << ( (*(mmdi.second)).first )->GetName() << G4endl;
    if( mmdi.first == mmdi.second ) { 
	//-second == theLVPVTree.end() ) {
      //this should never happen if there is no bug in DetRep
      G4cerr << "!!!EXITING GmGeometryUtils::AddParentPV " << pv->GetName() << " LV parent not found " << lvParent << " " << theLVPVTree.size() << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( lvParent != 0 ) if( GeomVerb(debugVerb) ) G4cout << " parent = " << lvParent->GetName() << G4endl;
#endif
      exit(1);
    }
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(testVerb) ){
      G4int iim = 0;
      for( mmcite = mmdi.first; mmcite != mmdi.second; mmcite++ ) {
	iim++;
	G4cout << " LV parent name: " << lvParent->GetName() << " : PV corresponding to LV parent = " << iim << G4endl;
      }
    }
#endif
    
    //t    G4int iim = 0;
    //---- Loop to all the PV's corresponding to LV parent
    for( mmcite = mmdi.first; mmcite != mmdi.second; mmcite++ ) {
      //t      G4cout << " $$$ iim " << iim++ << G4endl;
      G4VPhysicalVolume* pv0 = (*mmcite).second;
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " another PV with = LV as parent " << (*mmcite).first << " " << pv0->GetName() << " " << pv0->GetCopyNo() << G4endl;
#endif
      //--- check that it is the copy number that you want
      bool copyOK = CheckPVCopyNo( pv0, parentCopyNo );
      if( copyOK ){
	//--- add it to the list of ancestors
	vpv.push_back( pv0 );
	//--- look one level up
	AddParentPV( pv0, vpv, ancestorsRequested, ancestorLevel+1 );
	//--- when looking for next ancestors, vpv will be added a new element, but when looking to the next PV in this loop, the PV added should be deleted
	vpv.pop_back();
      }
    }
  } else {
    //if top volume, store this chain of ancestors
    theAncestorLines.push_back( vpv );
    return;
  }
}
 


//-----------------------------------------------------------------------
bool GmGeometryUtils::CheckPVCopyNo( const G4VPhysicalVolume* pv, G4int copyNo )
{
  bool copyOK = 0;
  if( copyNo == -1) { 
    copyOK = 1;
  } else {
  //--- Simple placement: check that PV's copy number = copyNo
    if( !pv->IsReplicated() ) { 
      if( copyNo == pv->GetCopyNo() ) {
	copyOK = 1;
      } 
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::CheckPVCopyNo " << copyNo << " =? " << pv->GetCopyNo() << " OK= " << copyOK << G4endl; 
#endif

      //--- Replica or parameterisation: PV::theCopyNo has no sense, a PV has many touchables, so we will take the PV always (later, when building the Cg4DrTouchable, we will get the touchable number copyNo), except if the copyNo we are asking for is bigger than the number of copies replicated
    } else {
      //    G4PVReplica* pvrep = static_cast<G4PVReplica*>(pv);
      EAxis axis;
      G4int nReplicas;
      G4double width;
      G4double offset;
      G4bool consuming;
      pv->GetReplicationData(axis, nReplicas, width, offset, consuming);
      if( copyNo < nReplicas ) {
	copyOK = 1;
      } 
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::CheckPVCopyNo(replica) " << copyNo << " =? " << nReplicas << " OK= " << copyOK << G4endl; 
#endif
    }
  }

  return copyOK;
}



//-----------------------------------------------------------------------
void GmGeometryUtils::ExpandCopyNoList( std::vector< std::vector<G4int> >& vvi, std::vector<G4int> vi, G4int level )
{
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::ExpandCopyNoList level: " << level << G4endl;
#endif
  G4int siz = vi.size();
  G4int ii, jj;
  std::vector< G4int > viNew = vi; 
  for( ii = level; ii < siz; ii++ ) {
    G4int iiNew;
    //---- if it is < -10, it means it has to be expanded: from 1 -13 0, you create three lists: 1 0 0, 1 1 0, 1 2 0 
    if( vi[ii] < -10 ) {
      iiNew = -vi[ii] - 10 - 1;
      for( jj = 0; jj < iiNew; jj++ ){
	viNew[ii] = jj;
	ExpandCopyNoList( vvi, viNew, level+1 );
      }
    } else {
      iiNew = vi[ii];
    }
    viNew[ii] = iiNew;
    //-       G4cout << ii << "  viNew " << viNew[ii]<< G4endl;
  }
  vvi.push_back( viNew );

  if(GeomVerb(debugVerb)) {
    G4cout << vvi.size() << " expandList copyNo: ";
    for( ii=0; ii < siz; ii ++ ){
      G4cout << " " << viNew[ii];
    }
    G4cout << G4endl;
  }

}


//-----------------------------------------------------------------------
void GmGeometryUtils::RebuildDictionaries()
{
  thePVs.clear();
  theLVPVTree.clear();
  thePVLVTree.clear();

  BuildDictionaries();
}
  

//-----------------------------------------------------------------------
void GmGeometryUtils::BuildDictionaries()
{
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator citepv;
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "@@@ GmGeometryUtils::BuildDictionaries()  N pvs  " << pvs->size() << G4endl;
#endif
  for( citepv = pvs->begin(); citepv != pvs->end(); citepv++ ) {
    //build multimap of PV name - list of PV*'s 
    thePVs.insert( mmspv::value_type( (*citepv)->GetName(), *citepv ) );
    //build multimap of LV* - list of its PV*'s
    theLVPVTree.insert( mmlvpv::value_type((*citepv)->GetLogicalVolume(), *citepv) );
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::BuildDictionaries  inserting theLVPVTree " << (*citepv)->GetLogicalVolume()->GetName() << " " << (*citepv)->GetName() << G4endl;
#endif
  }

  //build map of PV* - parent LV* : the only info in GEANT4 is from a LV, the list of its daughters PV's
  G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::iterator citelv;
  //  G4cout << " lvs size " << lvs->size() << G4endl;

  G4int ii, siz;
  for( citelv = lvs->begin(); citelv != lvs->end(); citelv++ ) {
    siz = (*citelv)->GetNoDaughters();
    for( ii=0; ii< siz; ii++ ) {
      thePVLVTree[(*citelv)->GetDaughter(ii)] = *citelv;
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmGeometryUtils::BuildDictionaries  inserting thePVLVTree " << (*citelv)->GetDaughter(ii)->GetName() << " " << (*citelv)->GetName() << G4endl;
#endif
  }
  }

}

//-----------------------------------------------------------------------
std::set<G4String> GmGeometryUtils::GetAllSDTypes()
{
 std::set<G4String> sdtypes;
 
 const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
 std::vector<G4LogicalVolume*>::const_iterator lvcite;
 for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
   if( (*lvcite)->GetSensitiveDetector() != 0 ){
#ifndef GAMOS_NO_VERBOSE
     if( GeomVerb(infoVerb) ) G4cout << (*lvcite)->GetName() << " GetAllSDTypes SD type " << (*lvcite)->GetSensitiveDetector()->GetPathName()  << G4endl;
#endif
     G4String sdtype = (*lvcite)->GetSensitiveDetector()->GetPathName();
     if( sdtype != "/" ) { // MFD for scoring
       sdtypes.insert( sdtype.substr(1,sdtype.length()-2) );
     }
   }
 }

 return sdtypes;

}

//-----------------------------------------------------------------------
G4double GmGeometryUtils::GetDistanceToOutNoDirection(const G4Track* aTrack)
{
  // G4double safety = aTrack->GetStep()->GetPreStepPoint()->GetSafety();
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( pv == 0 ) {
    G4cerr << " GmGeometryUtils::GetDistanceToOutNoDirection:  no PV associated to Track " << aTrack->GetPosition() << " particle" << aTrack->GetDefinition()->GetParticleName() << " energy " << aTrack->GetKineticEnergy() << G4endl;
    return  0.;
  }
  G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
  const G4VTouchable* touch = aTrack->GetTouchable();
  G4ThreeVector localPos = touch->GetHistory()->GetTopTransform().TransformPoint(aTrack->GetPosition());
  G4double d2out = solid->DistanceToOut( localPos );
  
  return d2out;
}


//-----------------------------------------------------------------------
G4double GmGeometryUtils::GetDistanceToOutDirection(const G4Track* aTrack)
{  
  // G4double safety = aTrack->GetStep()->GetPreStepPoint()->GetSafety();
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( pv == 0 ) {
    G4cerr << " GmGeometryUtils::GetDistanceToOutDirection:  no PV associated to Track " << aTrack->GetPosition() << " particle" << aTrack->GetDefinition()->GetParticleName() << " energy " << aTrack->GetKineticEnergy() << G4endl;
    return  0.;
  }
  G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
  const G4VTouchable* touch = aTrack->GetTouchable();
  G4ThreeVector localPos = touch->GetHistory()->GetTopTransform().TransformPoint(aTrack->GetPosition());
  G4ThreeVector localDir = touch->GetHistory()->GetTopTransform().TransformAxis(aTrack->GetMomentumDirection());

  G4double d2out = solid->DistanceToOut( localPos, localDir );
  
  return d2out;
}


//-----------------------------------------------------------------------
G4String GmGeometryUtils::GetG4TouchableLongName(G4TouchableHistory* touch)
{
  G4String tname = "";

  for( G4int ii = 0;; ii++ ){
    //    G4cout << touch->GetHistoryDepth() << " GetG4TouchableLongName " << ii << " " <<  touch->GetVolume()->GetName() << " " << touch->GetReplicaNumber() << G4endl; //GDEB
    tname = "/" +  touch->GetVolume()->GetName() + ":" + GmGenUtils::itoa(touch->GetReplicaNumber()) + tname;
    if( touch->GetHistoryDepth() == 0 ) break;
    touch->MoveUpHistory(1);
  }

  return tname;
}



//-----------------------------------------------------------------------
std::vector<G4Material*> GmGeometryUtils::GetMaterialsUsed()
{
  std::vector<G4Material*> mateVector;
  std::set<G4Material*> mateSet;

  //---- Assume all G4LogicalVolume's are part of the geometry treeH
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    mateSet.insert( (*lvcite)->GetMaterial() );
  }

  std::set<G4Material*>::const_iterator ites;
  for( ites = mateSet.begin(); ites != mateSet.end(); ites++ ){
    mateVector.push_back(*ites); 
  }

  return mateVector;
}


//--------------------------------------------------------------------
G4double GmGeometryUtils::GetCubicVolume( G4VPhysicalVolume* pv )
{
  G4double volume;
  if( !bPreCalculatedPhantomCubicVolumes) {
    if( pv->IsParameterised() ) {
      std::map<G4VPhysicalVolume*, std::map<G4int, G4double>* >::const_iterator ite = thePreCalculatedParameterisedCubicVolumes.find(pv);
      if( ite != thePreCalculatedParameterisedCubicVolumes.end()) {
	std::map<G4int, G4double>::const_iterator iteid = (*ite).second->find(pv->GetCopyNo());
	if( iteid != (*ite).second->end() ) {
	  volume = (*iteid).second;
	} else {
	  volume = CalculateParameterisedCubicVolume( pv, 1 );
	}
      } else {
	volume = CalculateParameterisedCubicVolume( pv, 0 );
      }
    } else {

      G4LogicalVolume* lv = pv->GetLogicalVolume();
      std::map<G4LogicalVolume*, G4double>::const_iterator ite = thePreCalculatedCubicVolumes.find(lv);
      if( ite != thePreCalculatedCubicVolumes.end() ) {
	volume = (*ite).second;
      } else {
	volume = CalculateCubicVolume(lv);
      }
    }
  } else {
    //-    volume = GmParallelToMassUA::GetCubicVolume( pv->GetCopyNo() );
  }

  //  G4VPhysicalVolume* pv2;
  //G4cout << pv2->GetName() << G4endl;

  return volume;
}

//------------------------------------------------------------------------
G4double GmGeometryUtils::CalculateCubicVolume( G4LogicalVolume* lv )
{
  G4double volume = lv->GetSolid()->GetCubicVolume();
  //substract the children volumes
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  G4PhysicalVolumeStore::const_iterator ite;
  for(ite = pvs->begin(); ite != pvs->end(); ite++ ){
    if( (*ite)->GetMotherLogical() == lv ) {
      volume -= (*ite)->GetLogicalVolume()->GetSolid()->GetCubicVolume();
    }
  }

  thePreCalculatedCubicVolumes[lv] = volume;

  return volume;

}

//------------------------------------------------------------------------
G4double GmGeometryUtils::CalculateParameterisedCubicVolume( G4VPhysicalVolume* pv, G4bool bPVexists )
{
  G4double volume;
  G4int copyNo = pv->GetCopyNo();
  G4PVParameterised* pvParam = (G4PVParameterised*)(pv);
  
  std::map<G4int,G4double>* parVols = 0;
  if( bPVexists ) {
    std::map<G4VPhysicalVolume*, std::map<G4int, G4double>* >::const_iterator ite = thePreCalculatedParameterisedCubicVolumes.find(pv);
    parVols = (*ite).second;
  }
  if( !parVols ){
    parVols = new std::map<G4int,G4double>;
    //    G4cout << " CalculateParameterisedCubicVolume " << pv->GetName() << " NEW parvols " << parVols->size() << G4endl;
  }
   
  //  G4cout << " CalculateParameterisedCubicVolume SIZE " << thePreCalculatedParameterisedCubicVolumes.size() << G4endl;
  //    G4cout << " CalculateParameterisedCubicVolume " << pv->GetName() << " parvols " << parVols->size() << G4endl;
  std::map<G4int, G4double>::const_iterator iteid = parVols->find( copyNo );
  if( iteid != parVols->end() ) {
    volume = (*iteid).second;
  //      G4cout << " CalculateParameterisedCubicVolume " << pv->GetName() << " volume in parVols " << volume << G4endl;
  } else {
    G4VSolid* solid = pvParam->GetParameterisation()->ComputeSolid( copyNo, pv );
//	G4cout << " CalculateParameterisedCubicVolume solid " << solid->GetName() << G4endl;
    volume = solid->GetCubicVolume();
    //    G4cout << " CalculateParameterisedCubicVolume solid volume " << copyNo << " = " << volume << G4endl;
    (*parVols)[copyNo] = volume;
    thePreCalculatedParameterisedCubicVolumes[pv] = parVols;
  }

  return volume;
}

//------------------------------------------------------------------------
G4String GmGeometryUtils::BuildTouchableName( const G4ThreeVector& pos )
{
  if( !theTouchable ) theTouchable = new G4TouchableHistory;

  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, theTouchable, false );
  
  G4String volName = "";
  unsigned int siz = theTouchable->GetHistoryDepth();
  for( int ii = siz; ii >= 0; ii-- ){
    G4VPhysicalVolume * pv = theTouchable->GetVolume( ii );
    volName += "/" + pv->GetName() + ":" + GmGenUtils::itoa(pv->GetCopyNo());
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(infoVerb) ) G4cout << pv << " GmGeometryUtils::BuildTouchableName vol " << volName << " pos " << pos << G4endl;
#endif
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(infoVerb) ) G4cout << " GmGeometryUtils::BuildTouchableName vol " << volName << " pos " << pos << G4endl;
#endif
  
  return volName;
}

//----------------------------------------------------------------
G4VPhysicalVolume* GmGeometryUtils::GetPVFromPos( G4ThreeVector pos )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 
  G4VPhysicalVolume* pv = touch->GetVolume();
  delete touch;
  return pv;
  
}  

//-----------------------------------------------------------------------
G4bool GmGeometryUtils::IsPhantomVolume( G4VPhysicalVolume* pv )
{
  EAxis axis;
  G4int nReplicas;
  G4double width,offset;
  G4bool consuming = false;
  pv->GetReplicationData(axis,nReplicas,width,offset,consuming);
  EVolume type = (consuming) ? kReplica : kParameterised;
  //  G4cout << " GmRegularParamUtils " << type << " " <<  pv->GetRegularStructureId() << G4endl; //GDEB
  if( type == kParameterised && pv->GetRegularStructureId() != 0 ) {   //=1 G4PhantomParameterisation =2 G4PartialPhantomParameterisation
    return TRUE;
  } else {
    return FALSE;
  }

} 


//-----------------------------------------------------------------------
std::vector<G4PhantomParameterisation*> GmGeometryUtils::GetPhantomParams(G4bool bMustExist)
{
  std::vector<G4PhantomParameterisation*> paramregs;

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV  " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl; //GDEB
    if( IsPhantomVolume( *cite ) ) {
      const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*cite);
      G4VPVParameterisation* param = pvparam->GetParameterisation();
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //      G4cout << "G4PhantomParameterisation volume found  " << (*cite)->GetName() << G4endl; //GDEB
      paramregs.push_back( static_cast<G4PhantomParameterisation*>(param) );
    }
  }
  
  if( paramregs.size() == 0 && bMustExist ) G4Exception("GmRegularParamUtils::GetPhantomParam",
					    "Wrong argument",
					    FatalErrorInArgument,
					    "No G4PhantomParameterisation found ");
  
  return paramregs;
  
}

//-----------------------------------------------------------------------
G4PhantomParameterisation* GmGeometryUtils::GetPhantomParam(G4bool bMustExist)
{
  G4PhantomParameterisation* paramreg = 0;

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    //    G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
    if( IsPhantomVolume( *cite ) ) {
      const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*cite);
      G4VPVParameterisation* param = pvparam->GetParameterisation();
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //    if( static_cast<const G4PhantomParameterisation*>(param) ){
      //      G4cout << "G4PhantomParameterisation volume found  " << (*cite)->GetName() << G4endl;
      paramreg = static_cast<G4PhantomParameterisation*>(param);
    }
  }
  
  if( !paramreg && bMustExist ) G4Exception("GmRegularParamUtils::GetPhantomParam",
					    "Wrong argument",
					    FatalErrorInArgument,
					    "No G4PhantomParameterisation found ");
  
  return paramreg;
  
}

