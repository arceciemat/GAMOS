#include "GmClassifierByLogicalVolume.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"

//------------------------------------------------------------------
GmClassifierByLogicalVolume::GmClassifierByLogicalVolume(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByLogicalVolume::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByLogicalVolume::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}


//------------------------------------------------------------------
int64_t GmClassifierByLogicalVolume::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4LogicalVolume* lv = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
  std::map<G4LogicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(lv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[lv] = index;
  } else {
    index = (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByLogicalVolume::GetIndexFromStep " << index << " lv " << lv->GetName() << G4endl;
#endif

  return index;
}

//------------------------------------------------------------------
int64_t GmClassifierByLogicalVolume::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  std::map<G4LogicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(lv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[lv] = index;
  } else {
    index = (*ite).second;
  }
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByLogicalVolume::GetIndex " << index << " lv " << lv->GetName() << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByLogicalVolume::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4LogicalVolume*,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByLogicalVolume::~GmClassifierByLogicalVolume()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByLogicalVolume " << theName << G4endl;
    std::map<G4LogicalVolume*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetName() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}
  


#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
//#include "GamosCore/GamosBase/Base/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmModuleJaws.hh"

//-------------------------------------------------------------
void GmClassifierByLogicalVolume::SetIndices( std::vector<G4String> wl )
{
  const std::vector<G4String> par;
  new GmModuleJaws(par);
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
//  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
	//GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
    //std::vector<G4LogicalVolume*> keys = GmGeometryUtils::GetInstance()->GetLogicalVolumes( wl[ii], 1, -1 );
/*    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4LogicalVolume* key = keys[jj];
      int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
  */  
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
