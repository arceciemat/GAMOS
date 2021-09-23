#include "GmClassifierByRegion.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Region.hh"

//---------------------------------------------------------------
GmClassifierByRegion::GmClassifierByRegion(G4String name) : GmVClassifier( name )
{
}

//---------------------------------------------------------------
void GmClassifierByRegion::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByRegion::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//---------------------------------------------------------------
int64_t GmClassifierByRegion::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4Region* reg = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();
  std::map<G4Region*,int64_t>::const_iterator ite = theIndexMap.find(reg);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[reg] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByRegion::GetIndexFromStep " << index << " reg " << reg->GetName() << G4endl;
#endif

  return index;
}


//---------------------------------------------------------------
int64_t GmClassifierByRegion::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;
 
  G4Region* reg = pv->GetLogicalVolume()->GetRegion();
  std::map<G4Region*,int64_t>::const_iterator ite = theIndexMap.find(reg);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[reg] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByRegion::GetIndex " << index << " reg " << reg->GetName() << G4endl;
#endif

  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByRegion::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4Region*,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first->GetName();
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByRegion::~GmClassifierByRegion()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByRegion " << theName << G4endl;
    std::map<G4Region*,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first->GetName() << " = " << (*ite).second << G4endl;
    }
  }
#endif
}

//-------------------------------------------------------------
void GmClassifierByRegion::SetIndices( std::vector<G4String> )
{
  theMaxIndex -= theIndexMap.size()+1;  
  
}
