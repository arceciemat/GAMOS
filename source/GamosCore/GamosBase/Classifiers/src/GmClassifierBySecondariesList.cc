#include "GmClassifierBySecondariesList.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <set>
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"

//------------------------------------------------------------------
GmClassifierBySecondariesList::GmClassifierBySecondariesList(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierBySecondariesList::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierBySecondariesList::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//------------------------------------------------------------------
int64_t GmClassifierBySecondariesList::GetIndexFromStep(const G4Step* )
{
  int64_t index;
  std::vector<G4Track*> secoTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  std::vector<G4Track*>::const_iterator itet;
  std::multiset<G4String> secoNames;
  for( itet = secoTracks.begin(); itet != secoTracks.end(); itet++ ){
    secoNames.insert( (*itet)->GetDefinition()->GetParticleName() );
  }
  G4String name = "";
  std::multiset<G4String>::const_iterator ites;
  for( ites = secoNames.begin(); ites != secoNames.end(); ites++ ){
    if( ites != secoNames.begin() ) name += "_";
    name += (*ites);
  }

  std::map<const G4String,int64_t>::const_iterator ite = theIndexMap.find(name);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1;
    theIndexMap[name] = index;
  } else {
    index = (*ite).second;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesList::GetIndexFromStep " << index << " Secondary_names " << name << G4endl;
#endif
  
  return index;
}

//------------------------------------------------------------------
int64_t GmClassifierBySecondariesList::GetIndexFromTrack(const G4Track* )
{
  return 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierBySecondariesList::GetIndex  0 " << G4endl;
#endif
  
}

//--------------------------------------------------------------
G4String GmClassifierBySecondariesList::GetIndexName(int64_t index)
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
GmClassifierBySecondariesList::~GmClassifierBySecondariesList()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierBySecondariesList " << theName << G4endl;
    std::map<const G4String,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << (*ite).first << " = " << (*ite).second << G4endl;
    }
  }
#endif
}

