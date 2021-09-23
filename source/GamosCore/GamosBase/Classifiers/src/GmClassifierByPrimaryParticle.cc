#include "GmClassifierByPrimaryParticle.hh"
#include "GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4ParticleDefinition.hh"

//-------------------------------------------------------------
GmClassifierByPrimaryParticle::GmClassifierByPrimaryParticle(G4String name) : GmVClassifier( name )
{
}

//-------------------------------------------------------------
void GmClassifierByPrimaryParticle::SetParameters( std::vector<G4String>& params)
{
  if(params.size() != 0 ){
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByPrimaryParticle::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//-------------------------------------------------------------
int64_t GmClassifierByPrimaryParticle::GetIndexFromStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetParentID() == 0 
      && aStep->GetTrack()->GetCurrentStepNumber() == 1 ) {
    thePrimaryParticle = aStep->GetTrack()->GetDefinition();
  }

  int64_t index;
  std::map<G4ParticleDefinition*,int64_t>::const_iterator ite = theIndexMap.find(thePrimaryParticle);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[thePrimaryParticle] = index;
  } else {
    index = (*ite).second;
  }
  return index;
}

//-------------------------------------------------------------
int64_t GmClassifierByPrimaryParticle::GetIndexFromTrack(const G4Track* aTrack)
{
  if( aTrack->GetParentID() == 0 
      && aTrack->GetCurrentStepNumber() == 1 ) {
    thePrimaryParticle = aTrack->GetDefinition();
  }

  int64_t index;
  std::map<G4ParticleDefinition*,int64_t>::const_iterator ite = theIndexMap.find(thePrimaryParticle);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[thePrimaryParticle] = index;
  } else {
    index = (*ite).second;
  }
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByPrimaryParticle::GetIndexName(int64_t index)
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
GmClassifierByPrimaryParticle::~GmClassifierByPrimaryParticle()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByPrimaryParticle " << theName << G4endl;
    std::map<G4ParticleDefinition*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetParticleName() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}  

//-------------------------------------------------------------
void GmClassifierByPrimaryParticle::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
    std::vector<G4ParticleDefinition*> keys = GmGetParticleMgr::GetInstance()->GetG4ParticleList( wl[ii] );
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4ParticleDefinition* key = keys[jj];
      int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
    
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
