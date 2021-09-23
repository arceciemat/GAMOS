#include "GmDistributionMgr.hh"
#include "GmVDistribution.hh"
#include "GmBaseVerbosity.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmDistributionFactory.hh"
#endif

GmDistributionMgr* GmDistributionMgr::theInstance = 0;

//----------------------------------------------------------------------
GmDistributionMgr::GmDistributionMgr()
{
}

//----------------------------------------------------------------------
GmDistributionMgr* GmDistributionMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmDistributionMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmDistributionMgr::~GmDistributionMgr()
{
}

//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::CreateDistribution( std::vector<G4String> params, G4bool bExists )
{
#ifdef ROOT5
  GmVDistribution* distrib = Reflex::PluginService::Create<GmVDistribution*>(params[1],params[0]);
#else
  GmVDistribution* distrib = GmDistributionFactory::get()->create(params[1],params[0]);
#endif
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::CreateDistribution NAME=" << params[0] << " CLASS=" << params[1] << " = " << distrib << G4endl;
#endif
  if( distrib != 0 ) {
    distrib->SetClass(params[1]);
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    distrib->SetParameters( params );
    AddDistribution( distrib );

  } else if( bExists ) {
    G4Exception(" GmDistributionMgr::CreateDistribution  distrib type not found ",params[1],FatalException,"Please check with 'SealPluginDump ' and '/gamos/distrib' commands");
  }

  return distrib;
}

//----------------------------------------------------------------------
void GmDistributionMgr::AddDistribution( GmVDistribution* distrib )
{
  std::map<G4String,GmVDistribution*>::const_iterator ite = theDistributions.find( distrib->GetName() );
  if( ite == theDistributions.end() ) {
    theDistributions[distrib->GetName()] = distrib;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::AddDistribution " << distrib->GetName() << G4endl;
#endif
  } else {
    G4Exception("GmDistributionMgr::Adddistrib"," Adding two distribs with the same name, review your '/gamos/distrib' commands",FatalErrorInArgument,distrib->GetName());
  }

}


//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::FindOrBuildDistribution(std::vector<G4String> params, G4bool bExists )
{
  GmVDistribution* distrib = GetDistributionFromList( params[0] );
  if( distrib != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::FindOrBuildDistribution distrib found " << params[0] << G4endl;
#endif
    return distrib;
  } else {
    return CreateDistribution( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVDistribution* GmDistributionMgr::GetDistributionFromList(const G4String& distribName ) const
{
  std::map<G4String,GmVDistribution*>::const_iterator ite = theDistributions.find( distribName );
  if( ite != theDistributions.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::GetDistributionFromList found " << distribName << G4endl;
#endif
    return (*ite).second;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmDistributionMgr::GetDistributionFromList NOT found " << distribName << G4endl;
#endif
    return 0;
  }
}


//----------------------------------------------------------------------
void GmDistributionMgr::DeleteDistributions()
{
  std::map<G4String,GmVDistribution*>::iterator ite;

  for( ite = theDistributions.begin(); ite != theDistributions.end(); ite++ ){
    delete (*ite).second;
  }

  theDistributions.clear();

}
