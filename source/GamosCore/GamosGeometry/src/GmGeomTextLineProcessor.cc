#include "GmGeomTextLineProcessor.hh"
#include "GmRegionCutsMgr.hh"
#include "GmModuleJaws.hh"
#include "GmModuleMLC.hh"
#include "GmModuleRangeModulator.hh"
#include "GmOpticalPropertiesMgr.hh"

#include "G4tgrUtils.hh"

//---------------------------------------------------------------
GmGeomTextLineProcessor::GmGeomTextLineProcessor() : G4tgrLineProcessor()
{
}


//---------------------------------------------------------------
G4bool GmGeomTextLineProcessor::ProcessLine( const std::vector<G4String>& wl )
{

  G4bool iret = G4tgrLineProcessor::ProcessLine( wl );

  G4String wl0 = wl[0];
  for( unsigned int ii = 0; ii < wl0.length(); ii++ ){
    wl0[ii] = toupper( wl0[ii] );
  }

  if( !iret ) {
    iret = 1; 
    //------------------------------- parameter number
    if( wl0 == ":REGION" ) {
      std::vector<G4String>::const_iterator ite = wl.begin()+1;
      std::vector<G4String> wlc;
      for( ; ite != wl.end(); ite++ ) { //loop skipping the first one
	wlc.push_back( *ite );
      }
      //      wlc = wlc.erase( wlc.begin() );
      GmRegionCutsMgr::GetInstance()->AddRegionData( wlc );

    } else if( wl0 == ":CUTS" ) {
      std::vector<G4String>::const_iterator ite = wl.begin()+1;
      std::vector<G4String> wlc;
      for( ; ite != wl.end(); ite++ ) { //loop skipping the first one
	wlc.push_back( *ite );
      }
      GmRegionCutsMgr::GetInstance()->AddRegionCuts( wlc );
    } else if( wl0 == ":MODULE" ) {
      GmVModule* module = 0;
      if( wl[1] == "JAWS" ) {
	module = new GmModuleJaws( wl );
      } else if( wl[1] == "MLC" ) {
	module = new GmModuleMLC( wl );
      } else if( wl[1] == "RANGE_MODULATOR" ) {
	module = new GmModuleRangeModulator( wl );
      } else {
	G4Exception("GmGeomTextLineProcessor::ProcessLine",
		    "Wrong argument",
		    FatalException,
		    ":MODULE argument should be JAWS, MLC or RANGE_MODULATOR ");
      } 
      module->SetLineProcessor(this);
      module->BuildObjects();
    } else if( wl0 == ":MATE_PROPERTIES_TABLE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_EQ, " :MATE_PROPERTIES_TABLE");
      GmOpticalPropertiesMgr::GetInstance()->CreateMaterialPropertiesTable( wl );
    } else if( wl0 == ":MATEPT_ADD_ENERGIES" ) {
      G4tgrUtils::CheckWLsize( wl, 3, WLSIZE_GE, " :MATEPT_ADD_ENERGIES");
      GmOpticalPropertiesMgr::GetInstance()->AddEnergiesToTable( wl );
    } else if( wl0 == ":MATEPT_ADD_PROPERTY" ) {
      G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_GE, " :MATEPT_ADD_PROPERTY");
      GmOpticalPropertiesMgr::GetInstance()->AddPropertyToTable( wl );
    } else if( wl0 == ":MATEPT_ADD_CONST_PROPERTY" ) {
      G4tgrUtils::CheckWLsize( wl, 4, WLSIZE_EQ, " :MATEPT_ADD_CONST_PROPERTY");
      GmOpticalPropertiesMgr::GetInstance()->AddConstPropertyToTable( wl );
    } else if( wl0 == ":MATEPT_ATTACH_TO_MATERIAL" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_MATERIAL");
      GmOpticalPropertiesMgr::GetInstance()->AttachTableToMaterial( wl );
    } else if( wl0 == ":MATEPT_ATTACH_TO_OPTICAL_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :MATEPT_ATTACH_TO_OPTICAL_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->AttachTableToOpsurface( wl );  
    } else if( wl0 == ":OPTICAL_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :OPTICAL_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateOpticalSurface( wl );
    } else if( wl0 == ":LOGICAL_BORDER_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_BORDER_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateLogicalBorderSurface( wl );
    } else if( wl0 == ":LOGICAL_SKIN_SURFACE" ) {
      G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_GE, " :LOGICAL_SKIN_SURFACE");
      GmOpticalPropertiesMgr::GetInstance()->CreateLogicalSkinSurface( wl );
    } else {
      iret = 0; 
    } 
  }
  
  return iret;
  
}
