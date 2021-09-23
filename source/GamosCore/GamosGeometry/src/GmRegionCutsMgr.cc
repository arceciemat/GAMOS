#include "GmRegionCutsMgr.hh"
#include "GmRegionData.hh"
#include "GmGeometryUtils.hh"
#include "GmGeomVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"
#include "G4LogicalVolume.hh"

//----------------------------------------------------------------------
GmRegionCutsMgr* GmRegionCutsMgr::theInstance = 0;

//----------------------------------------------------------------------
GmRegionCutsMgr* GmRegionCutsMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmRegionCutsMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmRegionCutsMgr::GmRegionCutsMgr()
{
}

//-----------------------------------------------------------------------
GmRegionCutsMgr::~GmRegionCutsMgr()
{
}

//-----------------------------------------------------------------------
void GmRegionCutsMgr::AddRegionData( const std::vector<G4String>& rd )
{

  if( rd.size() > 1 && FindRegionData( rd[0] ).size() != 0 ){
    G4Exception("GmRegionCutsMgr::AddRegionData","Error in argument",JustWarning,G4String("Region already exists: " + rd[0]).c_str() );
    return;
  }
  theRegionDatae.push_back( new GmRegionData( rd ) );

}

//-----------------------------------------------------------------------
void GmRegionCutsMgr::AddRegionCuts( const std::vector<G4String>& rc )
{
  if( rc.size() == 0 ) { 
    G4cerr << "GmRegionCutsMgr::AddRegionCuts must have 3 or 4 arguments : REGION_NAME  gamma_CUT e-_CUT (e+_CUT), it has only " << rc.size() << G4endl; 
    G4Exception("GmRegionCutsMgr::AddRegionCuts","Error in number of arguments",FatalErrorInArgument,GmGenUtils::itoa( rc.size() ) );
  }

  //Find region
  std::vector<GmRegionData*>::const_iterator iter;
  std::vector<GmRegionData*> regs = FindRegionData(rc[0]);

  if( regs.size() == 0 ){
    G4Exception("GmRegionCutsMgr::AddRegionCuts","Error in arguments",FatalErrorInArgument,G4String(" region does not exist: " + rc[0]).c_str());
  } 

  for( unsigned int ii = 0; ii < regs.size(); ii++) {
    regs[ii]->SetCutsData( rc );
  }
}

//-----------------------------------------------------------------------
std::vector<GmRegionData*> GmRegionCutsMgr::FindRegionData( const G4String& name)
{
  std::vector<GmRegionData*> regs;
  std::vector<GmRegionData*>::const_iterator iter;
  for( iter = theRegionDatae.begin(); iter != theRegionDatae.end(); iter++ ){
    if( GmGenUtils::AreWordsEquivalent( name , (*iter)->GetRegionName()) ) {
      regs.push_back(*iter);
    }
  }
  
  return regs; 
  
}


//-----------------------------------------------------------------------
void GmRegionCutsMgr::BuildRegions()
{
  std::vector<GmRegionData*>::const_iterator iter;
  std::vector<G4String>::const_iterator ites;
  std::vector<G4LogicalVolume*>::const_iterator itelv;
  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
  for( iter = theRegionDatae.begin(); iter != theRegionDatae.end(); iter++ ){
    G4Region* reg = new G4Region( (*iter)->GetRegionName() );
    std::vector<G4String> lvs = (*iter)->GetLVNames();
    for( ites = lvs.begin(); ites != lvs.end(); ites++ ){
      std::vector<G4LogicalVolume*> lvsv = geomUtils->GetLogicalVolumes( *ites, true );
      for( itelv = lvsv.begin(); itelv != lvsv.end(); itelv++ ){
	reg->AddRootLogicalVolume( *itelv );
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(infoVerb) ) G4cout << "BuildRegions " << reg->GetName() << " LV " << (*itelv)->GetName() << G4endl;
#endif
      }
    }
  }

}


//-----------------------------------------------------------------------
void GmRegionCutsMgr::BuildProductionCuts()
{
  std::vector<GmRegionData*>::const_iterator iter;
  G4RegionStore* regions = G4RegionStore::GetInstance();
  //----- loop to region datae
  for( iter = theRegionDatae.begin(); iter != theRegionDatae.end(); iter++ ){
    if( (*iter)->CutsAreSet() ) {
      G4Region* reg = regions->GetRegion( (*iter)->GetRegionName() );
      if( !reg ) { 
	G4Exception("GmRegionCutsMgr::BuildProductionCuts","Error in argument",FatalErrorInArgument,G4String("Region not found: " + (*iter)->GetRegionName()).c_str() );
      }	
      G4ProductionCuts* cuts = new G4ProductionCuts ;

      cuts->SetProductionCut((*iter)->GetGammaCut(),"gamma");
      cuts->SetProductionCut((*iter)->GetElectronCut(),"e-");
      cuts->SetProductionCut((*iter)->GetPositronCut(),"e+");
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(infoVerb) ) G4cout << " BuildProdRegionCuts " << cuts->GetProductionCut(0) << " " << cuts->GetProductionCut(1) << " reg " << reg->GetName() << G4endl;
#endif
      reg->SetProductionCuts(cuts);
    }
  }
}


//-----------------------------------------------------------------------
void GmRegionCutsMgr::BuildRegionIndices()
{
  if( theRegionIndices.size() == 0 ){
    G4RegionStore* regStore = G4RegionStore::GetInstance();
    std::vector<G4Region*>::const_iterator ite;
    for( ite = regStore->begin(); ite != regStore->end(); ite++ ){
      theRegionIndices[*ite] = theRegionIndices.size()-1;
    }
  }
}


//-----------------------------------------------------------------------
G4int GmRegionCutsMgr::GetRegionIndex( G4Region* reg )
{
  BuildRegionIndices();

  return (*(theRegionIndices.find(reg))).second;

}

//-----------------------------------------------------------------------
G4Region* GmRegionCutsMgr::GetIndexRegion( G4int index )
{
  BuildRegionIndices();

  G4Region* reg = 0;

  std::map<G4Region*,G4int>::const_iterator item;
  for( item = theRegionIndices.begin(); item != theRegionIndices.end(); item++ ){
    if( (*item).second == index ){
      reg = (*item).first;
      break; 
    }
  }

  return reg;

}
