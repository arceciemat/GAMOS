#include "GmClassifierByAncestors.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//---------------------------------------------------------------
GmClassifierByAncestors::GmClassifierByAncestors(G4String name) : GmVClassifier( name )
{
#ifdef WIN32
  theMaxIndex = int64_t(std::pow(double(2),int(16)-1)-1); // sizeof(int64_t) = 32 gives error 
#else
  theMaxIndex = int64_t(pow(double(2.),int(sizeof(int64_t)*8)-2)-1);
#endif
}

//---------------------------------------------------------------
void GmClassifierByAncestors::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 2 ){
    G4Exception("GmClassifierByAncestors::GmClassifierByAncestors"," Number of parameters different than 2",FatalErrorInArgument," Parameters should be NUMBER_OF_ANCESTOR_LEVELS MAX_NUMBER_COPIES_PER_LEVEL");
  }

  theNAncestors = GmGenUtils::GetInteger(params[0]);
  theNShift = GmGenUtils::GetInteger(params[1]);
  if( theNAncestors < 0 ) {
    G4Exception("GmClassifierByAncestors::SetParameters","NAncestor (first argument) has to be >= 0",FatalErrorInArgument,G4String("It is" + params[1]).c_str());
  }
  //  G4cout << this << " GmClassifierByAncestors::SetParameters NUMBER_OF_ANCESTOR_LEVELS " << theNAncestors << " MAX_NUMBER_COPIES_PER_LEVEL " << theNShift << G4endl;
}

//---------------------------------------------------------------
int64_t GmClassifierByAncestors::GetIndexFromStep(const G4Step* aStep)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  int64_t index = 0;
  int64_t nsiz = std::min(theNAncestors,th->GetHistoryDepth());
  for( int64_t ii = 0; ii < nsiz; ii++ ){
    index += int64_t(pow(float(theNShift), int(ii))) * th->GetReplicaNumber(ii);
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << ii << " GmClassifierByAncestors " << theName << " index= " << index << G4endl;
#endif
  }
  if(index > theMaxIndex ){
    G4Exception("GmClassifierByAncestors::GetIndexFromStep",
		"Index too big",
		FatalErrorInArgument,
		G4String("It is a int64_t, so it cannot be bigger than " + GmGenUtils::itoa(theMaxIndex)).c_str());
  }

  G4String name;
  if( theIndexNames.find(index) == theIndexNames.end() ){
    for( int64_t ii = nsiz-1; ii >= 0; ii-- ){
      name += th->GetVolume(ii)->GetName() + ":" + GmGenUtils::itoa( th->GetReplicaNumber(ii) );
      if( ii != 0 ) name+= "/";
    }
    theIndexNames[index] = name;
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << th->GetHistoryDepth() << " GmClassifierByAncestors theIndexNames = " << name << G4endl;
#endif
  }
  
  if( theIndexMap.size() != 0 ) {
    std::map<G4String,int64_t>::const_iterator ite = theIndexMap.find(name);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierByAncestors::GetIndexFromStep",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + name).c_str());
    } else {
      return (*ite).second;
    }
  }

  return index; 

}

//---------------------------------------------------------------
int64_t GmClassifierByAncestors::GetIndexFromTrack(const G4Track* aTrack)
{
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  int64_t index = 0;
  G4int nsiz = std::min(theNAncestors,th->GetHistoryDepth());
  for( G4int ii = 0; ii < nsiz; ii++ ){
    index += int64_t(pow(float(theNShift), int(ii))) * th->GetReplicaNumber(ii);
  }

  if(index > theMaxIndex ){
    G4Exception("GmClassifierByAncestors::GetIndex()",
		"Index too big",
		FatalErrorInArgument,
		G4String("It is a int64_t, so it cannot be bigger than " + GmGenUtils::itoa(theMaxIndex)).c_str());
  }

  G4String name;
  if( theIndexNames.find(index) == theIndexNames.end() ){
    for( int64_t ii = nsiz-1; ii >= 0; ii-- ){
      name += th->GetVolume(ii)->GetName() + ":" + GmGenUtils::itoa( th->GetReplicaNumber(ii) );
      if( ii != 0 ) name+= "/";
    }
    theIndexNames[index] = name;
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << th->GetHistoryDepth() << " GmClassifierByAncestors theIndexNames = " << name << G4endl;
#endif
  }
  
  if( theIndexMap.size() != 0 ) {
    std::map<G4String,int64_t>::const_iterator ite = theIndexMap.find(name);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierByAncestors::GetIndex",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + name).c_str());
    } else {
      return (*ite).second;
    }
  }

  return index; 

}


//--------------------------------------------------------------
G4String GmClassifierByAncestors::GetIndexName(int64_t index)
{
  std::map<int64_t,G4String>::const_iterator ite = theIndexNames.find(index);
  if( ite == theIndexNames.end() ){
    G4Exception(" GmClassifierByAncestors::GetIndexName",
		"Index not found",
		FatalErrorInArgument,
		G4String("INDESX= "+GmGenUtils::itoa(index)).c_str());
  }

  return (*ite).second;
}

//-------------------------------------------------------------
void GmClassifierByAncestors::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[wl[ii]] = index;
    if( theMaxIndex < index) theMaxIndex = index;
    theIndexNames[int64_t(GmGenUtils::GetValue(wl[ii]))] = wl[ii+1];
  }

  theMaxIndex -= theIndexMap.size()+1;  

}
