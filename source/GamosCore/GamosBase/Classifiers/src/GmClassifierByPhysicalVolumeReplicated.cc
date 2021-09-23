#include "GmClassifierByPhysicalVolumeReplicated.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4LogicalVolume.hh"

//----------------------------------------------------------------
GmClassifierByPhysicalVolumeReplicated::GmClassifierByPhysicalVolumeReplicated(G4String name) : GmVClassifier( name )
{
}

//----------------------------------------------------------------
void GmClassifierByPhysicalVolumeReplicated::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 0 ) {
    G4String parastr = "PARAMS: ";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByPhysicalVolumeReplicated::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 0, and they are "+parastr).c_str());
  }
}

//----------------------------------------------------------------
int64_t GmClassifierByPhysicalVolumeReplicated::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4String pvname = pv->GetName() + ":" + GmGenUtils::ftoa(pv->GetCopyNo());
  std::map<G4String,int64_t>::const_iterator ite = theIndexMap.find(pvname);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pvname] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolumeReplicated::GetIndexFromStep "  << " " << index << " pv " << pv->GetName() << ":" << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//----------------------------------------------------------------
int64_t GmClassifierByPhysicalVolumeReplicated::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t index;
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return 0;

  G4String pvname = pv->GetName() + ":" + GmGenUtils::ftoa(pv->GetCopyNo());
  std::map<G4String,int64_t>::const_iterator ite = theIndexMap.find(pvname);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[pvname] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhysicalVolumeReplicated::GetIndex "  << " " << index << " pv " << pv->GetName() << ":" << pv->GetCopyNo() << G4endl;
#endif
  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByPhysicalVolumeReplicated::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4String,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if((*ite).second == index ){
      name= (*ite).first;
      break;
    }
  }
  return name;
}

//---------------------------------------------------------------
GmClassifierByPhysicalVolumeReplicated::~GmClassifierByPhysicalVolumeReplicated()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByPhysicalVolumeReplicated " << theName << G4endl;
    std::map<G4String,int64_t>::const_iterator ite;
    for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
      G4cout << theName << ": " << (*ite).first << " = " << (*ite).second << G4endl;
    }
  }
#endif
}  

//-------------------------------------------------------------
void GmClassifierByPhysicalVolumeReplicated::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
  
/*    std::vector<G4String> keys = GmGeometryUtils::GetInstance()->GetPhysicalVolumeNames( wl[ii] );
    for( unsigned int jj = 0; jj < keys.size(); jj++ ){
      G4String key = keys[jj];
      int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
      theIndexMap[key] = index;
      if( theMaxIndex < index) theMaxIndex = index;
    }
  */  
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
