#include "GmClassifierByPhysicalVolume.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
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
G4int GmClassifierByPhysicalVolume::GetIndexFromStep(const G4Step* aStep)
{
  G4int index;
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  std::map<G4VPhysicalVolume*,G4int>::const_iterator ite = theIndexMap.find(pv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pv] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolume::GetIndexFromStep " << index << " pv " << pv->GetName() << " : " << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//----------------------------------------------------------------
G4int GmClassifierByPhysicalVolume::GetIndexFromTrack(const G4Track* aTrack)
{
  G4int index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;

  std::map<G4VPhysicalVolume*,G4int>::const_iterator ite = theIndexMap.find(pv);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pv] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolume::GetIndex " << index << " pv " << pv->GetName() << " : " << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByPhysicalVolume::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<G4VPhysicalVolume*,G4int>::const_iterator ite;
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
  G4cout << "%%%%% Table of indices for GmClassifierByPhysicalVolume " << theName << G4endl;
  std::map<G4VPhysicalVolume*,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << theName << ": " << (*ite).first->GetName() << ":" << (*ite).first->GetCopyNo() << " = " << (*ite).second << G4endl;
  }
}

//-------------------------------------------------------------
void GmClassifierByPhysicalVolume::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
/*    std::vector<G4VPhysicalVolume*> keys = GmGeometryUtils::GetInstance()->GetPhysicalVolumes( wl[ii] );
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4VPhysicalVolume* key = keys[jj];
      G4int index = G4int(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
  */  
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
