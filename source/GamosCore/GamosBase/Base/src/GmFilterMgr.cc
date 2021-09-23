#include "GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#endif

GmFilterMgr* GmFilterMgr::theInstance = 0;

//----------------------------------------------------------------------
GmFilterMgr::GmFilterMgr()
{
}

//----------------------------------------------------------------------
GmFilterMgr* GmFilterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmFilterMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmFilterMgr::~GmFilterMgr()
{
}

//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::CreateFilter( std::vector<G4String> params, G4bool bExists )
{
  GmVFilter* filter = 0;
#ifdef ROOT5
  filter = Reflex::PluginService::Create<GmVFilter*>(params[1],params[0]);
#else
  filter = GmFilterFactory::get()->create(params[1],params[0]);
#endif

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " CreateFilter NAME=" << params[0] << " CLASS=" << params[1] << " = " << filter << G4endl;
#endif
  if( filter != 0 ) {
    filter->SetClass(params[1]);
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    filter->SetParameters( params );
    AddFilter( filter );

  } else if( bExists ) {
    G4Exception(" GmFilterMgr::CreateFilter() filter type not found ",params[1],FatalException,"Please check with 'SealPluginDump ' and '/gamos/filter' commands");
  }

  return filter;
}

//----------------------------------------------------------------------
void GmFilterMgr::AddFilter( GmVFilter* filter )
{
  std::map<G4String,GmVFilter*>::const_iterator ite = theFilters.find( filter->GetName() );
  if( ite == theFilters.end() ) {
    theFilters[filter->GetName()] = filter;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::AddFilter " << filter->GetName() << G4endl;
#endif
  } else {
    G4Exception("GmFilterMgr::Addfilter"," Adding two filters with the same name, review your '/gamos/filter' commands",FatalErrorInArgument,filter->GetName());
  }

}


//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::FindOrBuildFilter(std::vector<G4String> params, G4bool bExists )
{
  GmVFilter* filter = GetFilterFromList( params[0] );
  if( filter != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::FindOrBuildFilter filter found " << params[0] << G4endl;
#endif
    return filter;
  } else {
    return CreateFilter( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::GetFilterFromList(const G4String& filterName ) const
{
  std::map<G4String,GmVFilter*>::const_iterator ite = theFilters.find( filterName );
  if( ite != theFilters.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::GetFilterFromList found " << filterName << G4endl;
#endif
    return (*ite).second;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::GetFilterFromList NOT found " << filterName << G4endl;
#endif
    return 0;
  }
}


//----------------------------------------------------------------------
void GmFilterMgr::DeleteFilters()
{
  std::map<G4String,GmVFilter*>::iterator ite;

  for( ite = theFilters.begin(); ite != theFilters.end(); ite++ ){
    delete (*ite).second;
  }

  theFilters.clear();

}
