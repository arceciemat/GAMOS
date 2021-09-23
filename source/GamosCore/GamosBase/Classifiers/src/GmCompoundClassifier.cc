#include "GmCompoundClassifier.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------
GmCompoundClassifier::GmCompoundClassifier(G4String name) : GmVClassifier( name )
{
  bIsCompound = TRUE;
  theNShift = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NShift",100));

}

//------------------------------------------------------------------
void GmCompoundClassifier::SetParameters( std::vector<G4String>& params )
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmCompoundClassifier::SetParameters",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be at least one parameter: CLASSIFIER_CLASS/NAME. There is no parameter");
  }

  GmClassifierMgr* classifierMgr = GmClassifierMgr::GetInstance();
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4String> paramsF; 
    paramsF.push_back( params[ii] );
    paramsF.push_back( params[ii] );
    GmVClassifier* classifier = classifierMgr->FindOrBuildClassifier(paramsF, true);
    theClassifiers.push_back(classifier);
  }
  if( pow(double(theNShift),int(theClassifiers.size())) > 1.E32 ) {
    G4Exception("GmCompoundClassifier::SetParameters",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Index may be bigger than pow(2,32). It is reserved : pow("+GmGenUtils::itoa(theNShift)+GmGenUtils::itoa(theClassifiers.size())+")").c_str());
  }
}

//------------------------------------------------------------------
int64_t GmCompoundClassifier::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index = 0;
  for( unsigned int ii = 0; ii < theClassifiers.size(); ii++ ) {
    int64_t indexii = theClassifiers[ii]->GetIndexFromStep(aStep);
    if( indexii >= theNShift ) {
      G4Exception("GmCompoundClassifier::GetIndexFromStep",
		  "Error",
		  FatalException,
		  (theName + " index of classifier " + GmGenUtils::itoa(indexii) + " is bigger than allowed : " + GmGenUtils::itoa(theNShift) + " Change it with ' /gamos/setParam " + theName + ":NShift  VALUE").c_str());
    }
    index += int64_t(pow(double(theNShift),int(ii)))*indexii;
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmCompoundClassifier::GetIndexFromStep index= " << index 
				   << " " << ii
				   << " : " <<  theClassifiers[ii]->GetName()
				   << " = " << indexii << G4endl;
#endif
  }
  
  return index;
}

//------------------------------------------------------------------
int64_t GmCompoundClassifier::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index = 0;
  for( unsigned int ii = 0; ii < theClassifiers.size(); ii++ ) {
    int64_t indexii = theClassifiers[ii]->GetIndexFromTrack(aTrack);
    if( indexii >= theNShift ) {
      G4Exception("GmCompoundClassifier::GetIndexFromStep",
		  "Error",
		  FatalException,
		  (theName + " index of classifier " + GmGenUtils::itoa(indexii) + " is bigger than allowed : " + GmGenUtils::itoa(theNShift) + " Change it with ' /gamos/setParam " + theName + ":NShift  VALUE").c_str());
    }
    index += int64_t(pow(double(theNShift),int(ii)))*indexii;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmCompoundClassifier::GetIndexFromTrack " << index 
				   << " " << theClassifiers[0]->GetName() 
				   << " " << theClassifiers[1]->GetName()
				   << " NShift= " << theNShift << G4endl;
#endif
 
  return index;
}

//------------------------------------------------------------------
int64_t GmCompoundClassifier::GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2)
{
  int64_t index = 0;
  for( unsigned int ii = 0; ii < theClassifiers.size(); ii++ ) {
    int64_t indexii = theClassifiers[ii]->GetIndexFromSecoTrack(aTrack1,aTrack2);
    if( indexii >= theNShift ) {
      G4Exception("GmCompoundClassifier::GetIndexFromStep",
		  "Error",
		  FatalException,
		  (theName + " index of classifier " + GmGenUtils::itoa(indexii) + " is bigger than allowed : " + GmGenUtils::itoa(theNShift) + " Change it with ' /gamos/setParam " + theName + ":NShift  VALUE").c_str());
    }
    index += int64_t(pow(double(theNShift),int(ii)))*indexii;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmCompoundClassifier::GetIndexFromSecoTrack " << index 
				   << " " << theClassifiers[0]->GetName() 
				   << " " << theClassifiers[1]->GetName()
				   << " NShift= " << theNShift << G4endl;
#endif
 
  return index;
}

//--------------------------------------------------------------
G4String GmCompoundClassifier::GetIndexName(int64_t index)
{
  G4String name = "\"";
  int64_t index1; 
  for( unsigned int ii = 0; ii < theClassifiers.size(); ii++ ) {
    index1 = (index/int64_t(pow(float(theNShift),int(ii))))%theNShift;
    name += theClassifiers[ii]->GetIndexName(index1);
    if( ii != theClassifiers.size()-1 ) name+= " ";
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) )  G4cout << " GmCompoundClassifier::GetIndexName " << theName
				      << " index " << index 
				      << " ii " << ii 
				      << " index1 " << index1 
				      << " name " << name << G4endl;
#endif
  }
  name += "\"";

  return name;
}

//-------------------------------------------------------------
void GmCompoundClassifier::SetIndices( std::vector<G4String> )
{
  G4Exception("GmCompoundClassifier::SetIndices",
	      "Error",
	      FatalException,
	      "It cannot be called for a compound classifier");
}

//-------------------------------------------------------------
GmCompoundClassifier::~GmCompoundClassifier()
{ 
  //print names of each index 
  G4cout << "%%%%% Compound classifier using indices from " << theClassifiers.size() << G4endl;

    /*	 << theClassifiers[0]->GetName() 
	 << " + " << theNShift << " * " << theClassifiers[1]->GetName() 
	 << G4endl; */
}
