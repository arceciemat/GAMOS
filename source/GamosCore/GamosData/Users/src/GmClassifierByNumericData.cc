#include "GmClassifierByNumericData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"

//---------------------------------------------------------------
GmClassifierByNumericData::GmClassifierByNumericData(G4String name) : GmVClassifier( name )
{
  theAllowOutOfLimits = GmParameterMgr::GetInstance()->GetNumericValue(theName+":AllowOutOfLimits",1 );
  //  G4cout << " GmClassifierByNumericData AllowOutOfLimits " << theName << " = " << theAllowOutOfLimits << G4endl;
}

//---------------------------------------------------------------
void GmClassifierByNumericData::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 4 ) {
    G4Exception(G4String(theName+"::SetParameters").c_str(),
		"Error in number of parameters",
  		JustWarning,
		G4String("There should be four arguments: DATA_NAME NBINS MIN MAX (IS_EXPONENTIAL_STEP), and there are "+GmGenUtils::itoa(params.size())).c_str());
  }

  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout <<  "GmClassifierByNumericData::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  theData.push_back( data );
  
  if( theData[0]->GetCType() == "char" ) {
    G4Exception(G4String(theName+"::SetParameters").c_str(),
		"Data is not of numeric type",
  		JustWarning,
		G4String("Data is " + params[0] + " Maybe you want to use GmClassifierByStringData?").c_str());
  }

  G4int nBins = G4int(GmGenUtils::GetValue( params[1] ));
  theMin = GmGenUtils::GetValue( params[2] );
  theMax = GmGenUtils::GetValue( params[3] );
  theStep = (theMax-theMin)/nBins;

  G4double dd= theMin;
  G4int ii = 0;
  for( ;; ) {
    int nn = theIndexMap.size();
    theIndexMap[dd] = nn;
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << ii <<  " GmClassifierByNumericData " << theName << " theIndexMap " << dd << " = " << theIndexMap[dd] << G4endl;
#endif
    if( (dd - theMax) > -1.E-9/(theMax-theMin) )break;
    dd += theStep;
    if(ii > 10000) {
      G4Exception(G4String(theName+"::SetParameters").c_str(),
		  "More than 10000 classifications",
		  FatalErrorInArgument,
		  G4String("Check your parameters: min= " + GmGenUtils::ftoa(theMin)+ 
			    " max= " + GmGenUtils::ftoa(theMax)+
			    " step= " + GmGenUtils::ftoa(theStep)));
    }
    ii++;
  }

}
  
//---------------------------------------------------------------
int64_t GmClassifierByNumericData::GetIndexFromStep(const G4Step* aStep)
{
  G4double val = theData[0]->GetValueFromStep( aStep );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
int64_t GmClassifierByNumericData::GetIndexFromTrack(const G4Track* aTrack)
{
  G4double val = theData[0]->GetValueFromTrack( aTrack );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
int64_t GmClassifierByNumericData::GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2)
{
  G4double val = theData[0]->GetValueFromSecoTrack( aTrack1, aTrack2 );
  //  G4cout << " GmClassifierByNumericData::GetIndexFromSecoTrack( " << val << G4endl; //GDEB
  return GetIndexFromValue(val);
}


//---------------------------------------------------------------
int64_t GmClassifierByNumericData::GetIndexFromValue(const G4double val )
{
  if( (val - theMin) < -1.E-5 || (val - theMax) > 1.E-5 ) {
    if( theAllowOutOfLimits == 1 ) {
      G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		  "Value out of limits",
		  JustWarning,
		  G4String(GmGenUtils::ftoa(theMin)+ " <=? " + GmGenUtils::ftoa(val)+" <=? "+GmGenUtils::ftoa(theMax)).c_str());
      if( val < theMin ) return 0;
      if( val > theMax ) return INT_MAX;
    } else if( theAllowOutOfLimits == 0 ) {
      G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		  "Value out of limits",
		  FatalErrorInArgument,
		  G4String(GmGenUtils::ftoa(theMin)+ " <=? " + GmGenUtils::ftoa(val)+" <=? "+GmGenUtils::ftoa(theMax)).c_str());
    } else {
      if( val < theMin ) return 0;
      if( val > theMax ) return INT_MAX;
    }
  }

  std::map<G4double,int64_t>::const_iterator ite = theIndexMap.lower_bound(val);
  int64_t index = (*ite).second;
  // Check for precision problems
  if( index == 0 && fabs(val-(*ite).second) < 1.E-6 ) index = 1;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << "GmClassifierByNumericData:: " << theName << " " << index << " " << val << G4endl;
#endif

  return index;
}

//---------------------------------------------------------------
G4String GmClassifierByNumericData::GetIndexName(int64_t index)
{
  G4String name;
  std::map<G4double,int64_t>::const_iterator ite, ite2;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    if( (*ite).second == index ) {
      ite2 = ite; ite2--;
      name = GmGenUtils::ftoa((*ite2).first)
	+ ":" + GmGenUtils::ftoa((*ite).first);
      //   G4cout << " GmClassifierByNumericData::GetIndexName " << (*ite2).first << " : " << (*ite2).second << G4endl; //GDEB
    }
  }

  return name;
}

//---------------------------------------------------------------
GmClassifierByNumericData::~GmClassifierByNumericData()
{
  //print names of each index 
#ifndef GAMOS_NO_VERBOSE
  if( ClassifierVerb(debugVerb) ) {
    G4cout << "%%%%% Table of indices for GmClassifierByNumericData " << theName << " " << theIndexMap.size() << G4endl;
    std::map<G4double,int64_t>::const_iterator ite1 = theIndexMap.begin();
    std::map<G4double,int64_t>::const_iterator ite2 = theIndexMap.begin();
    ite2++;
    G4int ii = 0;
    for(  ; ite2 != theIndexMap.end(); ite1++,ite2++,ii++ ){
      G4cout << theName << " = " << (*ite2).second << ": " << (*ite1).first << " - " << (*ite2).first << G4endl;
    }
  }
#endif
}


//-------------------------------------------------------------
void GmClassifierByNumericData::SetIndices( std::vector<G4String> wl )
{
  theMin = DBL_MAX;
  theMax = -DBL_MAX;
  theIndexMap.clear();
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    G4double value = GmGenUtils::GetValue(wl[ii]);
    theIndexMap[value] = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    if( theMin > value ) theMin = value;
    if( theMax < value ) theMax = value;
  }
  
}
