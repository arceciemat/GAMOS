#include "GmClassifierByMaterial.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4Material.hh"

//------------------------------------------------------------------
GmClassifierByMaterial::GmClassifierByMaterial(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByMaterial::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByMaterial::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}


//------------------------------------------------------------------
int64_t GmClassifierByMaterial::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4Material* mate = aStep->GetPreStepPoint()->GetMaterial();
  std::map<G4Material*,int64_t>::const_iterator ite = theIndexMap.find(mate);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+theMaxIndex;
    theIndexMap[mate] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByMaterial::GetIndexFromStep " << index << " mate " << mate->GetName() << G4endl;
#endif

  return index;
}

//------------------------------------------------------------------
int64_t GmClassifierByMaterial::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  G4Material* mate = aStep->GetPreStepPoint()->GetMaterial();
  std::map<G4Material*,int64_t>::const_iterator ite = theIndexMap.find(mate);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+theMaxIndex;
    theIndexMap[mate] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByMaterial::GetIndex " << index << " mate " << mate->GetName() << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByMaterial::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4Material*,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByMaterial::~GmClassifierByMaterial()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByMaterial " << theName << G4endl;
    std::map<G4Material*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetName() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}

//-------------------------------------------------------------
void GmClassifierByMaterial::SetIndices( std::vector<G4String> )
{
  theMaxIndex -= theIndexMap.size()+1;  
}
