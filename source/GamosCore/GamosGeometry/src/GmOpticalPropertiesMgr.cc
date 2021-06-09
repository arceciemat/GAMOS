#include "GmOpticalPropertiesMgr.hh"
#include "GmRegionData.hh"
#include "GmGeometryUtils.hh"
#include "GmGeomVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"
#include "G4LogicalVolume.hh"

#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"

//----------------------------------------------------------------------
GmOpticalPropertiesMgr* GmOpticalPropertiesMgr::theInstance = 0;

//----------------------------------------------------------------------
GmOpticalPropertiesMgr* GmOpticalPropertiesMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmOpticalPropertiesMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmOpticalPropertiesMgr::GmOpticalPropertiesMgr()
{
}

//-----------------------------------------------------------------------
GmOpticalPropertiesMgr::~GmOpticalPropertiesMgr()
{
}

//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateMaterialPropertiesTable( const std::vector<G4String> wl )
{
  if( theMatPropTables.find(wl[1]) == theMatPropTables.end() ) {
    theMatPropTables[wl[1]] = new G4MaterialPropertiesTable();
  } else {
    G4Exception(" GmOpticalPropertiesMgr::CreateMaterialPropertiesTable",
		" Repeated material properties table",
		FatalErrorInArgument,
		G4String("Repeated table: " + wl[1] ));
  }
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AddEnergiesToTable( std::vector<G4String> wl )
{
  std::vector<G4String>::iterator ite = wl.begin();
  ite++; ite++;
  wl.insert(ite, "ENERGIES" );
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) {
    for( unsigned int ii = 0; ii < wl.size() ; ii++ ) {
      G4cout << "GmOpticalPropertiesMgr::AddEnergiesToTable ENER " << wl[ii] << G4endl;
    }
  }
#endif
  AddPropertyToTable( wl );

}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AddPropertyToTable( const std::vector<G4String> wl )
{
  msmpt::const_iterator itet = theMatPropTables.find(wl[1]);

  if( itet != theMatPropTables.end() ) {
    // check if a map of properties for this table exists or not
    msmsvd::const_iterator itep = theMPTProperties.find(wl[1]); 
    if( itep != theMPTProperties.end() ) {
      //It exists: 1. check that vector has same length as other vectors in table

      // get first vector
      std::map<G4String,std::vector<G4double> >* firstProp = (*itep).second;
      std::vector<G4double> firstPropV = (*(firstProp->begin())).second;
      if( wl.size()-3 != firstPropV.size() ) {
	G4Exception(" GmOpticalPropertiesMgr::AddPropertyToTable",
		    " Trying to add a property of length different that a previous one",
		FatalErrorInArgument,
		    G4String("Table: " + wl[1] + " New property: " + wl[2] + " Old property: " + (*(firstProp->begin())).first ));
      }
    } else {
      //It does not exists: create map of properties for this table
      theMPTProperties[ wl[1] ] = new std::map<G4String,std::vector<G4double> >;
      itep = theMPTProperties.find(wl[1]); 
    }
      // 2. use this map 
    std::map<G4String,std::vector<G4double> >* mpvect = (*itep).second;

    std::vector<G4double> properties;
    for( unsigned int ii = 3; ii < wl.size(); ii++ ) {
      properties.push_back( GmGenUtils::GetValue( wl[ii] ) );
    }
    (*mpvect)[wl[2]] = properties; 
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) )  G4cout << " added to table " << wl[1] << " a property= " << wl[2] << " of size " << properties.size() << G4endl;
#endif
  } else {
    G4Exception(" GmOpticalPropertiesMgr::AddPropertyToTable",
		" Trying to add a property to a non existing material properties table",
		FatalErrorInArgument,
		G4String("Looking for table : " + wl[1] ));
  }

}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AddConstPropertyToTable( const std::vector<G4String> wl )
{
  msmpt::const_iterator itet = theMatPropTables.find(wl[1]);

  if( itet != theMatPropTables.end() ) {
    // check if a property for this table exists or not
    msmsd::const_iterator itep = theMPTConstProperties.find(wl[1]); 
    if( itep != theMPTConstProperties.end() ) {
    } else {
      //It does not exists: create properties for this table
      theMPTConstProperties[ wl[1] ] =  new std::map<G4String,G4double>;
      itep = theMPTConstProperties.find(wl[1]); 
    }

    std::map<G4String,G4double>* mpd = (*itep).second;
    (*mpd)[wl[2]] = GmGenUtils::GetValue(wl[3]);
  } else {
    G4Exception(" GmOpticalPropertiesMgr::AddConstPropertyToTable",
		" Trying to add a property to a non existing material properties table",
		FatalErrorInArgument,
		G4String("Looking for table : " + wl[1] ));
  }
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AttachTableToMaterial( const std::vector<G4String> wl )
{
  msmpt::const_iterator itet = theMatPropTables.find(wl[1]);

  if( itet != theMatPropTables.end() ) {
    theTableToMaterials[wl[1] ] = wl[2];
  } else {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToMaterial",
		" Trying to add a property to a non existing material properties table",
		FatalErrorInArgument,
		G4String("Looking for table : " + wl[1] ));
  }

}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateOpticalSurface( const std::vector<G4String> wl )
{
  if( theOpticalSurfaces.find(wl[1]) == theOpticalSurfaces.end() ) {
    G4OpticalSurfaceModel model = glisur;
    G4OpticalSurfaceFinish finish = polished;
    G4SurfaceType type = dielectric_dielectric;
    G4double polish = 1.0;
    G4double sigma_alpha = 0.;
    if( wl.size() > 2 ) {
      if( wl[2] == "glisur" || wl[2] == "GLISUR" ) {
	model = glisur;
      } else if( wl[2] == "unified" || wl[2] == "UNIFIED" ) {
	model = unified; 
      } else if( wl[2] == "LUT" ) {
	model = LUT;
      } else {
	G4Exception(" GmOpticalPropertiesMgr::CreateOpticalSurface",
		    " Wrong model",
		    FatalErrorInArgument,
		    G4String("Mode is: " + wl[2] + ", while it can only be glisur, unified or LUT"));
      }
    }
    
    if( wl.size() > 3 ) {
      if( wl[3] == "polished"){ finish = polished;
      } else if( wl[3] == "polishedfrontpainted"){ finish = polishedfrontpainted;
      } else if( wl[3] == "polishedbackpainted"){ finish = polishedbackpainted;
      } else if( wl[3] == "ground"){ finish = ground;
      } else if( wl[3] == "groundfrontpainted"){ finish = groundfrontpainted;
      } else if( wl[3] == "groundbackpainted"){ finish = groundbackpainted;
      } else if( wl[3] == "polishedlumirrorair"){ finish = polishedlumirrorair;
      } else if( wl[3] == "polishedlumirrorglue"){ finish = polishedlumirrorglue;
      } else if( wl[3] == "polishedair"){ finish = polishedair;
      } else if( wl[3] == "polishedteflonair"){ finish = polishedteflonair;
      } else if( wl[3] == "polishedtioair"){ finish = polishedtioair;
      } else if( wl[3] == "polishedtyvekair"){ finish = polishedtyvekair;
      } else if( wl[3] == "polishedvm2000air"){ finish = polishedvm2000air;
      } else if( wl[3] == "polishedvm2000glue"){ finish = polishedvm2000glue;
      } else if( wl[3] == "etchedlumirrorair"){ finish = etchedlumirrorair;
      } else if( wl[3] == "etchedlumirrorglue"){ finish = etchedlumirrorglue;
      } else if( wl[3] == "etchedair"){ finish = etchedair;
      } else if( wl[3] == "etchedteflonair"){ finish = etchedteflonair;
      } else if( wl[3] == "etchedtioair"){ finish = etchedtioair;
      } else if( wl[3] == "etchedtyvekair"){ finish = etchedtyvekair;
      } else if( wl[3] == "etchedvm2000air"){ finish = etchedvm2000air;
      } else if( wl[3] == "etchedvm2000glue"){ finish = etchedvm2000glue;
      } else if( wl[3] == "groundlumirrorair"){ finish = groundlumirrorair;
      } else if( wl[3] == "groundlumirrorglue"){ finish = groundlumirrorglue;
      } else if( wl[3] == "groundair"){ finish = groundair;
      } else if( wl[3] == "groundteflonair"){ finish = groundteflonair;
      } else if( wl[3] == "groundtioair"){ finish = groundtioair;
      } else if( wl[3] == "groundtyvekair"){ finish = groundtyvekair;
      } else if( wl[3] == "groundvm2000air"){ finish = groundvm2000air;
      } else if( wl[3] == "groundvm2000glue"){ finish = groundvm2000glue;
      } else {
	G4Exception(" GmOpticalPropertiesMgr::CreateOpticalSurface",
		    " Wrong finish",
		    FatalErrorInArgument,
		    G4String("Finish is: " + wl[3] + ", while it can only be polished, polishedfrontpainted, polishedbackpainted, ground, groundfrontpainted, groundbackpainted, polishedlumirrorair, polishedlumirrorglue, polishedair, polishedteflonair, polishedtioair, polishedtyvekair, polishedvm2000air, polishedvm2000glue, etchedlumirrorair, etchedlumirrorglue, etchedair, etchedteflonair, etchedtioair, etchedtyvekair, etchedvm2000air, etchedvm2000glue, groundlumirrorair, groundlumirrorglue, groundair, groundteflonair, groundtioair, groundtyvekair, groundvm2000air or groundvm2000glue"));
      }
    }
    
    if( wl.size() > 4 ){
      if( wl[4] == "dielectric_metal"){type = dielectric_metal;
      } else if( wl[4] == "dielectric_dielectric"){type = dielectric_dielectric;
      } else if( wl[4] == "dielectric_LUT"){type = dielectric_LUT;
      } else {
	G4Exception(" GmOpticalPropertiesMgr::CreateOpticalSurface",
		    " Wrong type",
		    FatalErrorInArgument,
		    G4String("Type is: " + wl[4] + ", while it can only be dielectric_metal, dielectric_dielectric, dielectric_LUT"));
      }
    }
    if( wl.size() > 5 ) {
      polish = GmGenUtils::GetValue( wl[5] );
    }
    if( wl.size() > 6 ) {
      sigma_alpha = GmGenUtils::GetValue( wl[6] );
    }

    theOpticalSurfaces[wl[1]] = new G4OpticalSurface(wl[1], model, finish, type, polish );
    theOpticalSurfaces[wl[1]]->SetSigmaAlpha(sigma_alpha);
  } else {
    G4Exception(" GmOpticalPropertiesMgr::CreateOpticalSurface",
		" Repeated optical surface",
		FatalErrorInArgument,
		G4String("Repeated optical surface: " + wl[1] ));
  }
}

  
//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AttachTableToOpsurface( const std::vector<G4String> wl )
{
  msmpt::const_iterator itet = theMatPropTables.find(wl[1]);
  msos::const_iterator iteo = theOpticalSurfaces.find(wl[2]);

  if( itet == theMatPropTables.end() ) {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToOpSurface",
		" Trying to attach an optical surface to a non-existing material properties table",
		FatalErrorInArgument,
		G4String("Optical surface: " + wl[2] + " table : " + wl[1] ));
  }else if( iteo == theOpticalSurfaces.end() ) {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToMaterial",
		" Trying to attach a non-existing optical surface to a material properties table",
		FatalErrorInArgument,
		G4String("Optical surface: " + wl[2] + " table : " + wl[1] ));
  } else {
    (*iteo).second->SetMaterialPropertiesTable( (*itet).second );
  }
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateLogicalBorderSurface( const std::vector<G4String> wl )
{
  if( theLogicalBorderSurfaceData.find(wl[1]) == theLogicalBorderSurfaceData.end() ) {
    msos::const_iterator ite = theOpticalSurfaces.find( wl[4] );
    if( ite == theOpticalSurfaces.end() ) {
      G4Exception(" GmOpticalPropertiesMgr::CreateLogicalBorderSurface",
		  " Optical surface does not exists",
		  FatalErrorInArgument,
		  G4String("optical surface: " + wl[4] + " in logical border surface " + wl[1]));
    }
      
    struct LogicalBorderSurfaceData lbs;
    lbs.PVname1 = wl[2];
    lbs.PVname2 = wl[3];
    lbs.surfaceProp = (*ite).second;    

    msos::const_iterator iteo;
    for( iteo = theOpticalSurfaces.begin(); iteo != theOpticalSurfaces.end(); iteo++ ){
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmOpticalPropertiesMgr::CreateLogicalBorderSurface " << wl[1] << " OOpticalSurface " << (*iteo).first << G4endl;
#endif
    }

#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) 
	G4cout << "GmOpticalPropertiesMgr::CreateLogicalBorderSurface  LogicalBorderData  " << lbs.PVname1 << " " << lbs.PVname2 << " " << lbs.surfaceProp << " " << (*ite).second << G4endl;
#endif

   theLogicalBorderSurfaceData[wl[1]] = lbs;
  } else {
    G4Exception(" GmOpticalPropertiesMgr::CreateLogicalBorderSurface",
		" Repeated logical border surface",
		FatalErrorInArgument,
		G4String("Repeated optical surface: " + wl[1] ));
  }
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateLogicalSkinSurface( const std::vector<G4String> wl )
{
  if( theLogicalSkinSurfaceData.find(wl[1]) == theLogicalSkinSurfaceData.end() ) {
    msos::const_iterator ite = theOpticalSurfaces.find( wl[4] );
    if( ite != theOpticalSurfaces.end() ) {
      G4Exception(" GmOpticalPropertiesMgr::CreateLogicalSkinSurface",
		  " Optical surface does not exists",
		  FatalErrorInArgument,
		  G4String("optical surface: " + wl[4] + " in logical skin surface " + wl[1]));
    }
      
    struct LogicalSkinSurfaceData lss;
    lss.LVname = wl[2];
    lss.surfaceProp = (*ite).second;    
    
    theLogicalSkinSurfaceData[wl[1]] = lss;
  } else {
    G4Exception(" GmOpticalPropertiesMgr::CreateLogicalSkinSurface",
		" Repeated logical skin surface",
		FatalErrorInArgument,
		G4String("Repeated optical surface: " + wl[1] ));
  }
}

//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::BuildG4()
{
  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();


  //----- Add properties to material properties tables
  msmsvd::const_iterator itep;
  for( itep = theMPTProperties.begin(); itep != theMPTProperties.end(); itep++ ){
    std::map<G4String,std::vector<G4double> >* prop = (*itep).second;
    std::map<G4String,std::vector<G4double> >::const_iterator itevE = prop->find("ENERGIES");
    if( itevE == prop->end() ){
      G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		   "Trying to add a property to a G4MaterialPropertiesTable wihtout having defined an energy vector",
		   FatalErrorInArgument,
		   G4String("Table: "+(*itep).first));
    }

    G4MaterialPropertiesTable* mpt = (*(theMatPropTables.find( (*itep).first ))).second;
    G4int nVal = (*itevE).second.size();
    G4double* enerD = new G4double[nVal];
    G4double* propD = new G4double[nVal];
    std::map<G4String,std::vector<G4double> >::const_iterator itev;
    for( itev = prop->begin(); itev != prop->end(); itev++ ){
      for( G4int ii = 0; ii < nVal; ii++ ){
	enerD[ii] = ((*itevE).second)[ii];
	propD[ii] = ((*itev).second)[ii];
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) 
      G4cout << "GmOpticalPropertiesMgr::BuildG4 add property " << (*itev).first<< " " << enerD[ii] << " " << propD[ii] <<" " << nVal <<G4endl;
#endif
      }
      mpt->AddProperty((*itev).first, enerD, propD, nVal );
    }
  
  }


  //----- Add const properties to material properties tables
  msmsd::const_iterator itecp;
  for( itecp = theMPTConstProperties.begin(); itecp != theMPTConstProperties.end(); itecp++ ){
    std::map<G4String,G4double>* prop = (*itecp).second;

    G4MaterialPropertiesTable* mpt = (*(theMatPropTables.find( (*itecp).first ))).second;
    std::map<G4String,G4double>::const_iterator ited;
    for( ited = prop->begin(); ited != prop->end(); ited++ ){
      mpt->AddConstProperty((*ited).first, (*ited).second );
    }
  
  }

  //----- Attach material properties tables to materials
  std::map<G4String, G4String>::const_iterator itet; 
  for( itet = theTableToMaterials.begin(); itet != theTableToMaterials.end(); itet++ ){
    // look for material 
    G4Material* mate = GmGeometryUtils::GetInstance()->GetMaterial( (*itet).second, false );
    if( mate == 0 ) {
      G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		   "Trying to attach a G4MaterialPropertiesTable to a non existing material",
		   FatalErrorInArgument,
		   G4String("Material:"+(*itet).second));
    } else {
      mate->SetMaterialPropertiesTable((*(theMatPropTables.find((*itet).first))).second);
    }
  }

  //----- Create logical border surfaces
  mslbsd::const_iterator itelb;
  for(itelb = theLogicalBorderSurfaceData.begin(); itelb != theLogicalBorderSurfaceData.end(); itelb++ ){
    LogicalBorderSurfaceData surfData = (*itelb).second;
    std::vector<G4VPhysicalVolume*> vPV1 = geomUtils->GetPhysicalVolumes( surfData.PVname1, false );
    if( vPV1.size() == 0 ) {
      G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		   "Trying to create a G4LogicalBorderSurface for a non-existing physical volume",
		   FatalErrorInArgument,
		   G4String("Logical Border Surface: "+(*itelb).first + " Physical Volume: " + surfData.PVname1));
    }
    std::vector<G4VPhysicalVolume*> vPV2 = geomUtils->GetPhysicalVolumes( surfData.PVname2, false );
    if( vPV2.size() == 0 ) {
      G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		   "Trying to create a G4LogicalBorderSurface for a non-existing physical volume",
		   FatalErrorInArgument,
		   G4String("Logical Border Surface: "+(*itelb).first + " Physical Volume: " + surfData.PVname2));
    }
    for( unsigned int i1 = 0; i1 < vPV1.size(); i1++ ) {
      for( unsigned int i2 = 0; i2 < vPV2.size(); i2++ ) {
	G4LogicalBorderSurface* lbs = new G4LogicalBorderSurface((*itelb).first,
				   vPV1[i1],
				   vPV2[i2],
				   surfData.surfaceProp );
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) 
	G4cout << "GmOpticalPropertiesMgr::BuildG4 G4LogicalBorderSurface " << lbs << " " << vPV1[i1]->GetName() << " " << vPV2[i1]->GetName() << " " << surfData.surfaceProp  << " = " << lbs->GetSurfaceProperty() << G4endl;
#endif

      }
    }
    
  }
  
  //----- Create logical skin surfaces
  mslssd::const_iterator itels;
  for(itels = theLogicalSkinSurfaceData.begin(); itels != theLogicalSkinSurfaceData.end(); itels++ ){
    LogicalSkinSurfaceData surfData = (*itels).second;
    std::vector<G4LogicalVolume*> vLV = geomUtils->GetLogicalVolumes( surfData.LVname, false );
    if( vLV.size() == 0 ) {
      G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		   "Trying to create a G4LogicalSkinSurface for a non-existing logical volume",
		   FatalErrorInArgument,
		   G4String("Logical Skin Surface: "+(*itels).first + " Logical Volume: " + surfData.LVname));
    }
    for( unsigned int ii = 0; ii < vLV.size(); ii++ ) {
      new G4LogicalSkinSurface((*itels).first,
			       vLV[ii],
			       surfData.surfaceProp );
      
    }
  }

}
