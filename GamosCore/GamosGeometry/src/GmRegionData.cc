
#include "GmRegionData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
GmRegionData::GmRegionData(const std::vector<G4String>& wl )
{
  if( wl.size() < (unsigned int)(2) ) { 
    G4Exception("GmRegionData::GmRegionData","Error in arguments",FatalErrorInArgument,GmGenUtils::itoa( wl.size() ) );
  }

  theRegionName = wl[0];
  for( unsigned int ii = 1; ii < wl.size(); ii++ ){
    theLVNames.push_back( wl[ii] );
  }

  bCutsSet = false;
}

//-----------------------------------------------------------------------
GmRegionData::~GmRegionData()
{
}


//-----------------------------------------------------------------------
void GmRegionData::SetCutsData( const std::vector<G4String>& rc )
{
  if( rc.size() != (unsigned int)(3) && rc.size() != (unsigned int)(4) ) { 
    G4cerr << "GmRegionData::SetCutsData must have 3 or 4 arguments : REGION_NAME  gamma_CUT e-_CUT (e+_CUT), it has only " << rc.size() << G4endl; 
    G4Exception("GmRegionCutsMgr::AddRegionCuts","Error in number of arguments",FatalErrorInArgument,GmGenUtils::itoa( rc.size() ) );
  }

 if( bCutsSet ) {
    G4Exception("GmRegionData::SetCuts","Error in argument",JustWarning,G4String("Cuts are already set for region " + theRegionName).c_str() );
  }

  theGammaCut = GmGenUtils::GetValue( rc[1] );
  theElectronCut = GmGenUtils::GetValue( rc[2] );
  if( rc.size() == (unsigned int)(3) ) {
    thePositronCut = theElectronCut;
  } else {
    thePositronCut = GmGenUtils::GetValue( rc[3] );
  }

  bCutsSet = true;
}

