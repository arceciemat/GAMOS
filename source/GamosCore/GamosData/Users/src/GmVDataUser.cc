#include "GmVDataUser.hh"
#include "GamosCore/GamosData/Management/include/GmVData2DHisto.hh"
#include "GamosCore/GamosData/Management/include/GmVDataProf1DHisto.hh"
#include "GamosCore/GamosData/Management/include/GmVDataProf2DHisto.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


#include "G4tgrUtils.hh"

//----------------------------------------------------------------
GmVDataUser::GmVDataUser()
{
}


//----------------------------------------------------------------
void GmVDataUser::BuildData()
{ 
  bUseAtInitial = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theUserName+":UseAtInitial",0));

  theDataList = GmParameterMgr::GetInstance()->GetVStringValue(theUserName+":DataList",theDataList);

  theData.clear();

  for( unsigned int ii = 0; ii < theDataList.size(); ii++) {
    // check if it is single or double data
    G4int ivs = theDataList[ii].find(".vs.");
    G4int iprof = theDataList[ii].find(".prof.");
    
    //      G4cout << " ivs " << ivs << " iprof " << iprof << " " << theDataList[ii] << G4endl;
    if( ivs == G4int(std::string::npos) && iprof == G4int(std::string::npos) ) 
      {
	G4String dataName = theDataList[ii];
	GmVData* data = Build1Data( dataName );
#ifndef GAMOS_NO_VERBOSE 
	if( DataVerb(infoVerb) ) G4cout <<  "GmVDataUser::BuildData 1D " << dataName << G4endl;
#endif
	data->SetName( theDataList[ii] );
	theData.push_back( data );
      } 
    else  if( ivs != G4int(std::string::npos) && iprof == G4int(std::string::npos) ) 
      {
	G4String dataName1 = theDataList[ii].substr(0,ivs);
	G4String dataName2 = theDataList[ii].substr(ivs+4,theDataList[ii].length());
#ifndef GAMOS_NO_VERBOSE
	if( DataVerb(infoVerb) ) G4cout <<  "GmVDataUser::BuildData 2D " << dataName1 << " " << dataName2 << G4endl;
#endif
	
	theData.push_back( BuildData2D(dataName1, dataName2) );
      }
    else  if( ivs == G4int(std::string::npos) && iprof != G4int(std::string::npos) ) 
      {
	G4String dataName1 = theDataList[ii].substr(0,iprof);
	G4String dataName2 = theDataList[ii].substr(iprof+6,theDataList[ii].length());
	
#ifndef GAMOS_NO_VERBOSE
	if( DataVerb(infoVerb) ) G4cout <<  "GmVDataUser::BuildData Prof1D " << dataName1 << " " << dataName2 << G4endl;
#endif
	theData.push_back( BuildDataProf1D(dataName1, dataName2) );
      }
    else  if( ivs != G4int(std::string::npos) && iprof != G4int(std::string::npos) ) 
      {
	G4String dataName1 = theDataList[ii].substr(0,ivs);
	G4String dataName2 = theDataList[ii].substr(ivs+4,iprof-ivs-4);
	G4String dataName3 = theDataList[ii].substr(iprof+6,theDataList[ii].length());
	
#ifndef GAMOS_NO_VERBOSE
	if( DataVerb(infoVerb) ) G4cout << "GmVDataUser::BuildData Prof2D " << dataName1 << " " << dataName2 << " " << dataName3 << G4endl;
#endif
	theData.push_back( BuildDataProf2D(dataName1, dataName2, dataName3) );
      }
  } 
  
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":DataList" << theDataList.size() << G4endl;
#endif
  
}

//----------------------------------------------------------------
GmVData* GmVDataUser::BuildData2D( const G4String& dataName1, const G4String& dataName2 )
{

  GmVData* data1 = Build1Data(dataName1);
  data1->SetName( dataName1 );
  GmVData* data2 = Build1Data(dataName2);
  data2->SetName( dataName2 );

  GmVData* data = new GmVData2DHisto();
  data->SetName( dataName1 + ".vs." + dataName2 );
  data->AddSubData( data1 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildData2D AddSubdata " << dataName1 << " data1 " << data1 << " " << data1->GetName() << G4endl;
#endif
  data->AddSubData( data2 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildData2D AddSubdata " << dataName2 << " data2 " << data2 << " " << data2->GetName() << G4endl;
#endif

  return data;
}


//----------------------------------------------------------------
GmVData* GmVDataUser::BuildDataProf1D( const G4String& dataName1, const G4String& dataName2 )
{

  GmVData* data1 = Build1Data(dataName1);
  data1->SetName( dataName1 );
  GmVData* data2 = Build1Data(dataName2);
  data2->SetName( dataName2 );

  GmVData* data = new GmVDataProf1DHisto();
  data->SetName( dataName1 + ".prof." + dataName2 );
  data->AddSubData( data1 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildDataProf1D AddSubdata " << dataName1 << " data1 " << data1 << " " << data1->GetName() << G4endl;
#endif
  data->AddSubData( data2 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildDataProf1D AddSubdata " << dataName2 << " data2 " << data2 << " " << data2->GetName() << G4endl;
#endif

  return data;
}


//----------------------------------------------------------------
GmVData* GmVDataUser::BuildDataProf2D( const G4String& dataName1, const G4String& dataName2, const G4String& dataName3 )
{

  GmVData* data1 = Build1Data(dataName1);
  data1->SetName( dataName1 );
  GmVData* data2 = Build1Data(dataName2);
  data2->SetName( dataName2 );
  GmVData* data3 = Build1Data(dataName3);
  data3->SetName( dataName3 );

  GmVData* data = new GmVDataProf2DHisto();
  data->SetName( dataName1 + ".vs." + dataName2 + ".prof." + dataName3 );
  data->AddSubData( data1 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildDataProf2D AddSubdata " << dataName1 << " data1 " << data1 << " " << data1->GetName() << G4endl;
#endif
  data->AddSubData( data2 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildDataProf2D AddSubdata " << dataName2 << " data2 " << data2 << " " << data2->GetName() << G4endl;
#endif
  data->AddSubData( data3 );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUser::BuildDataProf2D AddSubdata " << dataName3 << " data3 " << data3 << " " << data3->GetName() << G4endl;
#endif

  return data;
}


//----------------------------------------------------------------
G4String GmVDataUser::GetDataTypeAsString() const
{
  switch (theDataType) {
  case DTStep:
    return "DTStep";
    break;
  case DTTrack:
    return "DTTrack";
    break;
  case DTSeco:
    return "DTSeco";
    break;
  case DTEvent:
    return "DTEvent";
    break;
  case DTRun:
    return "DTRun";
    break;
  }

  return "";  
}


//----------------------------------------------------------------
GmVData* GmVDataUser::Build1Data( const G4String& dataName ) 
{
  GmVData* data = GmDataMgr::GetInstance()->BuildData(dataName);

  if( ! data->CheckAvailability( theDataType ) ) {
    G4Exception("GmVDataUser::GmVDataUser",
		G4String("Data type not available for " + GetDataTypeAsString()).c_str(),
		FatalErrorInArgument,
		G4String("Asking for data: " + dataName).c_str());
  } 

  return data;
} 
