#include "GmClassifierByPhysicalVolume.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"

//----------------------------------------------------------------
GmClassifierByPhysicalVolume::GmClassifierByPhysicalVolume(G4String name) : GmVClassifier( name )
{
}

//----------------------------------------------------------------
void GmClassifierByPhysicalVolume::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByPhysicalVolume::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//----------------------------------------------------------------
int64_t GmClassifierByPhysicalVolume::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  std::map<G4VPhysicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(pv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pv] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolume::GetIndexFromStep " << index << " pv " << pv->GetName() << " : " << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//----------------------------------------------------------------
int64_t GmClassifierByPhysicalVolume::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;

  std::map<G4VPhysicalVolume*,int64_t>::const_iterator ite = theIndexMap.find(pv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pv] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolume::GetIndex " << index << " pv " << pv->GetName() << " : " << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByPhysicalVolume::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4VPhysicalVolume*,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetName()+":"+GmGenUtils::itoa((*ite).first->GetCopyNo());
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByPhysicalVolume::~GmClassifierByPhysicalVolume()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByPhysicalVolume " << theName << G4endl;
    std::map<G4VPhysicalVolume*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetName() << ":" << (*ite).first->GetCopyNo() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}  

//-------------------------------------------------------------
void GmClassifierByPhysicalVolume::SetIndices( std::vector<G4String> )
{
  theMaxIndex -= theIndexMap.size()+1;  
}
