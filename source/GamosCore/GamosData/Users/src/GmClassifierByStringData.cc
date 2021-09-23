#include "GmClassifierByStringData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//------------------------------------------------------------------
GmClassifierByStringData::GmClassifierByStringData(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByStringData::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 1 ) {
    G4String parastr = "";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByStringData::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 1: DATA_NAME, and they are: "+parastr).c_str());
  }

  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout <<  "GmClassifierByStringData::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  theData.push_back( data );
 
  if( theData[0]->GetCType() != "char" ) {
    G4Exception("GmClassifierByStringData::SetParameters",
		"Data is not of string type",
  		JustWarning,
		G4String("Data is " + params[0] + " Maybe you want to use GmClassifierByNumericData?").c_str());
  }

}


//------------------------------------------------------------------
int64_t GmClassifierByStringData::GetIndexFromStep(const G4Step* aStep)
{
  G4String val = theData[0]->GetStringValueFromStep( aStep );
  return GetIndexFromValue( val );
}

//------------------------------------------------------------------
int64_t GmClassifierByStringData::GetIndexFromTrack(const G4Track* aTrack)
{
  G4String val = theData[0]->GetStringValueFromTrack( aTrack );
  return GetIndexFromValue( val );
}


//---------------------------------------------------------------
int64_t GmClassifierByStringData::GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2)
{
  G4String val = theData[0]->GetStringValueFromSecoTrack( aTrack1, aTrack2 );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
int64_t GmClassifierByStringData::GetIndexFromValue(const G4String val )
{
  int64_t index;
  std::map<G4String,int64_t>::const_iterator ite = theIndexMap.find(val);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[val] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmClassifierByStringData::GetIndexFromValue " << theName << " : " << index << " key " << val << " index " << index << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByStringData::GetIndexName(int64_t index)
{
  G4String name = "NOT_FOUND";
  std::map<G4String,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    //    G4cout << " (*ite).second == index " << (*ite).second << " == " << index << G4endl;
    if((*ite).second == index ){
      name = (*ite).first;
      break;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmClassifierByStringData::GetIndexName " << theName << " : " << name << " index " << index << G4endl;
#endif
  return name;
}

//---------------------------------------------------------------
GmClassifierByStringData::~GmClassifierByStringData()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByStringData " << theName << G4endl;
  std::map<G4String,int64_t>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << theName << ": " << (*ite).first << " = " << (*ite).second << G4endl;
  }
}

//-------------------------------------------------------------
void GmClassifierByStringData::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[wl[ii]] = index;
    if( theMaxIndex < index) theMaxIndex = index;
  }
    
  theMaxIndex -= theIndexMap.size()+1;  
}
