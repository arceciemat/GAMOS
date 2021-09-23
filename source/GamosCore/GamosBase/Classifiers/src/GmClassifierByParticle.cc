#include "GmClassifierByParticle.hh"
#include "GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
//---------------------------------------------------------------
GmClassifierByParticle::GmClassifierByParticle(G4String name) : GmVClassifier( name )
{
}

//---------------------------------------------------------------
void GmClassifierByParticle::SetParameters( std::vector<G4String>& params)
{
  if(params.size() != 0 ){
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii < params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByParticle::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//---------------------------------------------------------------
int64_t GmClassifierByParticle::GetIndexFromStep(const G4Step* aStep)
{
  return GetIndexFromTrack( aStep->GetTrack() );
}


//---------------------------------------------------------------
int64_t GmClassifierByParticle::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  G4ParticleDefinition* part = aTrack->GetDefinition();
  std::map<G4ParticleDefinition*,int64_t>::const_iterator ite = theIndexMap.find(part);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[part] = index;
  } else {
    index = (*ite).second;
  }
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByParticle::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4ParticleDefinition*,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetParticleName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByParticle::~GmClassifierByParticle()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByParticle " << theName << G4endl;
    std::map<G4ParticleDefinition*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetParticleName() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}

//-------------------------------------------------------------
void GmClassifierByParticle::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
    std::vector<G4ParticleDefinition*> keys = GmGetParticleMgr::GetInstance()->GetG4ParticleList( wl[ii] );
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4ParticleDefinition* key = keys[jj];
      int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
      //      G4cout << " key " << key->GetParticleName() << " " <<  GmGenUtils::GetValue(wl[ii+1]) << G4endl;
    }
    
  }
 
  theMaxIndex -= theIndexMap.size()+1;   
}
