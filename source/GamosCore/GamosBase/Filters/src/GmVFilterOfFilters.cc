#include "GmVFilterOfFilters.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"

//----------------------------------------------------------------
GmVFilterOfFilters::GmVFilterOfFilters(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------
GmVFilterOfFilters::~GmVFilterOfFilters()
{ 
}

//----------------------------------------------------------------
void GmVFilterOfFilters::show()
{
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
    theFilters[ii]->show();
  }
}

//----------------------------------------------------------------
void GmVFilterOfFilters::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVFilterOfFilters::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4String> paramsF; 
    paramsF.push_back( params[ii] );
    paramsF.push_back( params[ii] );
    GmVFilter* filter = filterMgr->FindOrBuildFilter(paramsF, true);
    theFilters.push_back(filter);
  }
}
