#include "GmClassifierByCreatorProcess.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"

//------------------------------------------------------------------
GmClassifierByCreatorProcess::GmClassifierByCreatorProcess(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByCreatorProcess::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByProces::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//------------------------------------------------------------------
int64_t GmClassifierByCreatorProcess::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  const G4VProcess* proc = aStep->GetTrack()->GetCreatorProcess();
  G4String procName = "Primary";
  if( proc ) {
    procName = proc->GetProcessName();
  } 
  std::map<const G4String,int64_t>::const_iterator ite = theIndexMap.find(procName);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[procName] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByCreatorProcess::GetIndexFromStep " << index << " proc " << procName << G4endl;
#endif
  
  return index;
}

//------------------------------------------------------------------
int64_t GmClassifierByCreatorProcess::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  const G4VProcess* proc = aTrack->GetCreatorProcess();
  G4String procName = "Primary";
  if( proc ) {
    procName = proc->GetProcessName();
  }
  std::map<const G4String,int64_t>::const_iterator ite = theIndexMap.find(procName);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[procName] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByCreatorProcess::GetIndex " << index << " proc " << procName << G4endl;
#endif
  
  return index;
}

//--------------------------------------------------------------
G4String GmClassifierByCreatorProcess::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4String,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      return (*ite).first;
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByCreatorProcess::~GmClassifierByCreatorProcess()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByCreatorProcess " << theName << G4endl;
    std::map<const G4String,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << (*ite).first << " = " << (*ite).second << G4endl;
    }
  }
  #endif
}

//-------------------------------------------------------------
void GmClassifierByCreatorProcess::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
    std::vector<G4VProcess*> keys = GmGetParticleMgr::GetInstance()->GetG4ProcessList( wl[ii]);
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4VProcess* key = keys[jj];
      int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key->GetProcessName()] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
    
  }
  
  theMaxIndex -= theIndexMap.size()+1;  
}
