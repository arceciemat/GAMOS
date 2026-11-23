#include "GmOpticalPropertiesMgr.hh"
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
void GmOpticalPropertiesMgr::AddWavelengthsToTable( std::vector<G4String> wl )
{
  std::vector<G4String>::iterator ite = wl.begin();
  ite++; ite++;
  wl.insert(ite, "WAVELENGTHS" );
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) {
    for( unsigned int ii = 0; ii < wl.size() ; ii++ ) {
      G4cout << "GmOpticalPropertiesMgr::AddWavelengthsToTable WAVELENGTH " << wl[ii] << G4endl;
    }
  }
#endif
  AddPropertyToTable( wl );

}

//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AddPropertyToTable( const std::vector<G4String> wl )
{
  for( size_t ii = 0; ii < wl.size(); ii++ ) {
    G4cout << ii << " AddPropertyToTable " << wl[ii] << G4endl;
  }
  G4bool bFound = false;
  for( msmpt::const_iterator itet = theMatPropTables.begin(); itet != theMatPropTables.end(); itet++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], itet->first) ) {
      bFound = true;
    }
  }
  if( bFound ) {      
    // check if a map of properties for this table exists or not
    G4bool bFound2 = false; 
    for( msmsvd::const_iterator itep = theMPTProperties.begin(); itep != theMPTProperties.end(); itep++ ) {
      if( GmGenUtils::AreWordsEquivalent(wl[1], itep->first) ) {
	bFound2 = true;
	//It exists: 1. check that vector has same length as other vectors in table
	// get first vector
	std::map<G4String,std::vector<G4double> >* firstProp = (*itep).second;
	
	// check that the energy or wavelength vector is not defined twice
	if (wl[2]==(*(firstProp->begin())).first) {
	  G4Exception(" GmOpticalPropertiesMgr::AddPropertyToTable",
		      " Trying to add the wavelength or energy vector twice.",
		      FatalErrorInArgument,
		      G4String("Table: " + wl[1] ));  
	}
	std::vector<G4double> firstPropV = (*(firstProp->begin())).second;
	if( wl.size()-3 != firstPropV.size() ) {
	  G4Exception(" GmOpticalPropertiesMgr::AddPropertyToTable",
		      " Trying to add a property of length different that a previous one",
		      FatalErrorInArgument,
		      G4String("Table: " + wl[1] + " New property: " + wl[2] + " Old property: " + (*(firstProp->begin())).first ));
	}
      }
    }
    if( !bFound2 ) {
      //It does not exists: create map of properties for this table
      theMPTProperties[ wl[1] ] = new std::map<G4String,std::vector<G4double> >;
    }

    // 2. use the map(s)
    std::map<G4String,std::vector<G4double> >* mpvect = theMPTProperties[ wl[1] ];
    std::vector<G4double> properties;
    for( unsigned int ii = 3; ii < wl.size(); ii++ ) {
      properties.push_back( GmGenUtils::GetValue( wl[ii] ) );
    }
    (*mpvect)[wl[2]] = properties; 
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) )  G4cout << "GmOpticalPropertiesMgr::AddPropertyToTable( added to table " << wl[1] << " a property= " << wl[2] << " of size " << properties.size() << G4endl;
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
  G4bool bFound = false;
  for( msmpt::const_iterator itet = theMatPropTables.begin(); itet != theMatPropTables.end(); itet++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], itet->first) ) {
      bFound = true;
    }
  }
  if( bFound ) {
    G4bool bFound2 = false;
    // check if a property for this table exists or not
    for( msmsd::const_iterator itep = theMPTConstProperties.begin(); itep != theMPTConstProperties.end(); itep++ ) {
      if( GmGenUtils::AreWordsEquivalent(wl[1], itep->first) ) {
	bFound2 = true;
      }
    }
    
    if( !bFound2 ) {
      //It does not exists: create properties for this table
      theMPTConstProperties[ wl[1] ] = new std::map<G4String,G4double>;
    }

    std::map<G4String,G4double>* mpd = theMPTConstProperties[ wl[1] ];
   
    (*mpd)[wl[2]] = GmGenUtils::GetValue(wl[3]);

#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << "GmOpticalPropertiesMgr::AddConstPropertyToTable(  added to table " << wl[1] << " a property= " << wl[2] << " value= " << wl[3] << " N=" << mpd->size() << G4endl;
#endif

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
  G4bool bFound = false;
  for( msmpt::const_iterator itet = theMatPropTables.begin(); itet != theMatPropTables.end(); itet++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], itet->first) ) {
      bFound = true;
      theTableToMaterials[wl[1] ] = wl[2];
    }
  }
  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToMaterial",
		" Trying to add a property to a non existing material properties table",
		FatalErrorInArgument,
		G4String("Looking for table : " + wl[1] ));
  }
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << this << " GmOpticalPropertiesMgr::AttachTableToMaterial " << wl[2] << " -> " << wl[1] << " N=" << theTableToMaterials.size() << " " << theMatPropTables.size() << G4endl;
#endif

  
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateOpticalSurface( const std::vector<G4String> wl )
{
  G4bool bFound = false;
  for( msos::const_iterator iteo = theOpticalSurfaces.begin(); iteo != theOpticalSurfaces.end(); iteo++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], iteo->first) ) {
      bFound = true;
    }
  }
  
  if( !bFound ) {
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
  }

  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::CreateOpticalSurface",
		" Repeated optical surface",
		FatalErrorInArgument,
		G4String("Repeated optical surface: " + wl[1] ));
  }
}

  
//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::AttachTableToOpsurface( const std::vector<G4String> wl )
{
  G4bool bFound = false;
  for( msmpt::const_iterator itet = theMatPropTables.begin(); itet != theMatPropTables.end(); itet++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], itet->first) ) {
      bFound = true;
      break;
    }
  } 
  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToOpSurface",
		" Trying to attach an optical surface to a non-existing material properties table",
		FatalErrorInArgument,
		G4String("Optical surface: " + wl[2] + " table : " + wl[1] ));
  }

  bFound = false;
  for( msmpt::const_iterator itet = theMatPropTables.begin(); itet != theMatPropTables.end(); itet++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[1], itet->first) ) {
      for( msos::const_iterator iteo = theOpticalSurfaces.begin(); iteo != theOpticalSurfaces.end(); iteo++ ) {
	if( GmGenUtils::AreWordsEquivalent(wl[2], iteo->first) ) {
	  bFound = true;
	  (*iteo).second->SetMaterialPropertiesTable( (*itet).second );
	}
      }
    }
  }
  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::AttachTableToMaterial",
		" Trying to attach a non-existing optical surface to a material properties table",
		FatalErrorInArgument,
		G4String("Optical surface: " + wl[2] + " table : " + wl[1] ));
  }
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateLogicalBorderSurface( const std::vector<G4String> wl )
{
  G4bool bFound = false;
  for( mslbsd::const_iterator itelb = theLogicalBorderSurfaceData.begin(); itelb != theLogicalBorderSurfaceData.end(); itelb++ ){
    if( GmGenUtils::AreWordsEquivalent(wl[1], itelb->first) ) {    
      bFound = true;
      G4Exception(" GmOpticalPropertiesMgr::CreateLogicalBorderSurface",
		  " Repeated logical border surface",
		  FatalErrorInArgument,
		  G4String("Repeated optical surface: " + wl[1] ));
    }
  }

  // Check if material property table exists with same name
  msmpt::iterator itemp = theMatPropTables.find(wl[1]);
  if( itemp == theMatPropTables.end() ) {
    G4Exception(" GmOpticalPropertiesMgr::CreateLogicalBorderSurface",
		" Material property table does not exists",
		FatalErrorInArgument,
		G4String("Material property table " + wl[1]));
  }
  
  // Check if optical surface exists
  bFound = false;
  for( msos::const_iterator ite = theOpticalSurfaces.begin(); ite != theOpticalSurfaces.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent(wl[4], ite->first) ) {
      bFound = true; 
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
    }
  }
  
  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::CreateLogicalBorderSurface",
		" Optical surface does not exists",
		FatalErrorInArgument,
		G4String("optical surface: " + wl[4] + " in logical border surface " + wl[1]));
  }
  
}


//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::CreateLogicalSkinSurface( const std::vector<G4String> wl )
{
  for( mslssd::const_iterator itels = theLogicalSkinSurfaceData.begin(); itels != theLogicalSkinSurfaceData.end(); itels++ ){
    if( GmGenUtils::AreWordsEquivalent(wl[1], itels->first) ) {    
      G4Exception(" GmOpticalPropertiesMgr::CreateLogicalSkinSurface",
		  " Repeated logical skin surface",
		  FatalErrorInArgument,
		  G4String("Repeated optical surface: " + wl[1] ));
    }
  }

    // Check if optical surface exists
  G4bool bFound = false;
  for( msos::const_iterator ite = theOpticalSurfaces.begin(); ite != theOpticalSurfaces.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent(wl[3], ite->first) ) {
      bFound = true;
      struct LogicalSkinSurfaceData lss;
      lss.LVname = wl[2];
      lss.surfaceProp = (*ite).second;    

      theLogicalSkinSurfaceData[wl[1]] = lss;
    }
  }
        
  if( !bFound ) {
    G4Exception(" GmOpticalPropertiesMgr::CreateLogicalSkinSurface",
		  " Optical surface does not exists",
		  FatalErrorInArgument,
		G4String("optical surface: " + wl[3] + " in logical skin surface " + wl[1]));
  }

  
}

//-----------------------------------------------------------------------
void GmOpticalPropertiesMgr::BuildG4()
{
  //  G4cout << " GmOpticalPropertiesMgr::BuildG4() " << G4endl; //GDEB
  //  G4cout << this << " GmOpticalPropertiesMgr::BuildG4 N=" << theTableToMaterials.size() << " " << theMatPropTables.size() << G4endl; //GDEB

  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();

  //----- Add properties to material properties tables
  msmsvd::const_iterator itep;
  for( itep = theMPTProperties.begin(); itep != theMPTProperties.end(); itep++ ){
    std::map<G4String,std::vector<G4double> >* prop = (*itep).second;
    std::map<G4String,std::vector<G4double> >::const_iterator itevE1 = prop->find("ENERGIES");
    std::map<G4String,std::vector<G4double> >::const_iterator itevE2 = prop->find("WAVELENGTHS");
        std::map<G4String,std::vector<G4double> >::const_iterator itevE;
    if( itevE1 == prop->end() ){
      if( itevE2 == prop->end() ){
        G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		     "Trying to add a property to a G4MaterialPropertiesTable wihtout having defined an energy vector",
		     FatalErrorInArgument,
		     G4String("Table: "+(*itep).first));
      }
      else{
        itevE = itevE2;
      }
    } else if (itevE1 != prop->end() ){
      if ( itevE2 != prop->end() ){
	G4Exception( "GmOpticalPropertiesMgr::BuildG4 ",
		     "Trying to add a property to a G4MaterialPropertiesTable with both an energy and wavelength vector.",
		     FatalErrorInArgument,
		     G4String("Table: "+(*itep).first));
      }
      else{
        itevE = itevE1;
      }
    } else {
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
    std::vector<G4Material*> mates = GmGeometryUtils::GetInstance()->GetAllMaterials( (*itet).second, true );
    for( size_t iim = 0; iim < mates.size(); iim++ ) {
      G4Material* mate = mates[iim];
      mate->SetMaterialPropertiesTable((*(theMatPropTables.find((*itet).first))).second);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) 
	G4cout << "GmOpticalPropertiesMgr::BuildG4 SetMaterialPropertiesTable TO " << mate->GetName() << G4endl;
#endif
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
    // Attach material property table to surface
    G4MaterialPropertiesTable* matProp = (*(theMatPropTables.find((*itelb).first))).second;
    G4OpticalSurface* surfPropOptical = (G4OpticalSurface*)(surfData.surfaceProp);
    surfPropOptical->SetMaterialPropertiesTable(matProp);
    
    // Create a surface per each pair of volumes
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

    // Attach material property table to surface
    G4MaterialPropertiesTable* matProp = (*(theMatPropTables.find((*itelb).first))).second;
    G4OpticalSurface* surfPropOptical = (G4OpticalSurface*)(surfData.surfaceProp);
    surfPropOptical->SetMaterialPropertiesTable(matProp);

    // Create a surface per volume
    for( unsigned int ii = 0; ii < vLV.size(); ii++ ) {
      new G4LogicalSkinSurface((*itels).first,
			       vLV[ii],
			       surfData.surfaceProp );
      
    }
  }

}
