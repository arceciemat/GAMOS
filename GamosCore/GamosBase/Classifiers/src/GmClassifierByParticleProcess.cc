#include "GmClassifierByParticleProcess.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"


//------------------------------------------------------------------
GmClassifierByParticleProcess::GmClassifierByParticleProcess(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByParticleProcess::SetParameters( std::vector<G4String>& params)
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
G4int GmClassifierByParticleProcess::GetIndexFromStep(const G4Step* aStep)
{
  G4int index;
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  std::map<const G4VProcess*,G4int>::const_iterator ite = theIndexMap.find(proc);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[proc] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByParticleProcess::GetIndexFromStep " << index << " proc ";
#endif
  if( proc ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << proc->GetProcessName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "OutOfWorld" << G4endl;
#endif
  }

  return index;
}
//------------------------------------------------------------------
G4int GmClassifierByParticleProcess::GetIndexFromTrack(const G4Track* aTrack)
{
  G4int index;
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();

  std::map<const G4VProcess*,G4int>::const_iterator ite = theIndexMap.find(proc);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[proc] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByParticleProcess::GetIndex " << index << " proc ";
#endif
  if( proc ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << proc->GetProcessName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "OutOfWorld" << G4endl;
#endif
  }

  return index;
}

//--------------------------------------------------------------
G4String GmClassifierByParticleProcess::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<const G4VProcess*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      const G4VProcess* proc = (*ite).first;
      if( proc ) {
	name = proc->GetProcessName();
      } else {
	name = "OutOfWorld";
      }
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByParticleProcess::~GmClassifierByParticleProcess()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByParticleProcess " << theName << G4endl;
  std::map<const G4VProcess*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    const G4VProcess* proc = (*ite).first;
    G4cout << theName << ": ";
    if( proc ) {
      G4ParticleDefinition* part = GmGetParticleMgr::GetInstance()->GetG4Particle(proc);
      G4cout << part->GetParticleName() << " " << proc->GetProcessName();
    } else {
      G4cout << "OutOfWorld";
    }
    G4cout << " = " << (*ite).second << G4endl;
  }
}

//---------------------------------------------------------
void GmClassifierByParticleProcess::CheckNumberOfIndices( std::vector<G4String> wl )
{
  if( wl.size()%3 != 0 ) {
    G4Exception("GmClassifierByParticleProcess::SetIndices",
		("Wrong number of parameters in classifier " + theName).c_str(),
		FatalErrorInArgument,
		"There should be a multiple of 3 parameters after classifier name in command /gamos/base/setClassifierIndices : PARTICLE_NAME_1 PROCESS_NAME_1 INDEX_2 PARTICLE_NAME_2 PROCESS_NAME_2 INDEX_2 ...");
  }
}

//-------------------------------------------------------------
void GmClassifierByParticleProcess::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=3 ){
  
    std::vector<G4VProcess*> keys = GmGetParticleMgr::GetInstance()->GetG4ProcessList( wl[ii], wl[ii+1]);
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4VProcess* key = keys[jj];
      G4int index = G4int(GmGenUtils::GetValue(wl[ii+2]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
    
  }

 theMaxIndex -= theIndexMap.size()+1;    
}
