#include "GmClassifierByHadronicReaction.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4Gamma.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4tgrFileIn.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"

//------------------------------------------------------------------
GmClassifierByHadronicReaction::GmClassifierByHadronicReaction(G4String name) : GmVClassifier( name )
{
  theIonTable = G4ParticleTable::GetParticleTable()->GetIonTable();
}

//------------------------------------------------------------------
void GmClassifierByHadronicReaction::SetParameters( std::vector<G4String>& params)
{
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::SetParameters " << params.size() << G4endl;
#endif
  /*
  for( unsigned int ii=0; ii < params.size(); ii++ ){
    theParticlesToExclude.insert(params[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::SetParameters particle to exclude  " << params[ii] << " " << theParticlesToExclude.size() << G4endl;
#endif 
  }
  */

}

//------------------------------------------------------------------
int64_t GmClassifierByHadronicReaction::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4String indexName;

  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep process " << proc << G4endl;
#endif
  if( !proc ) return 0;
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep process type " << proc->GetProcessType() << " fHadronic " << fHadronic << G4endl;
#endif

  if( proc->GetProcessType() != fHadronic ) return 0;

  G4String procName = proc->GetProcessName();
  //other processes than inelastic
  if( procName.find("inelastic") != std::string::npos || procName.find("Inelastic") != std::string::npos 
      || procName.find("nCapture") != std::string::npos ) {
    
    indexName = GmG4Utils::GetInelasticName( aStep );
  }

  std::map<const G4String,int64_t>::const_iterator ite = theIndexMap.find(indexName);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1;
    theIndexMap[indexName] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::GetIndexFromStep " << index << " Secondary_names " << indexName << G4endl;
#endif
  
  return index;
}


//------------------------------------------------------------------
int64_t GmClassifierByHadronicReaction::GetIndexFromTrack(const G4Track* )
{
  return 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByHadronicReaction::GetIndex  0 " << G4endl;
#endif

}


//--------------------------------------------------------------
G4String GmClassifierByHadronicReaction::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4String,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      return (*ite).first;
      break;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) 
    G4cout << " GmClassifierByHadronicReaction::GetIndexName " << name << " index " << index << G4endl;
#endif

  return name;
}

//---------------------------------------------------------------
GmClassifierByHadronicReaction::~GmClassifierByHadronicReaction()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByHadronicReaction " << theName << G4endl;
    std::map<const G4String,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << (*ite).first << " = " << (*ite).second << G4endl;
    }
  }
  #endif
}

