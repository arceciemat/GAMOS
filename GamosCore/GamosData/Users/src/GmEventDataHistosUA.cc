#include "GmEventDataHistosUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmEventDataHistosUA::GmEventDataHistosUA()
{
  theDataType = DTEvent;

}


//----------------------------------------------------------------
void GmEventDataHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("GmEventDataHistosUA",theFilters,theClassifier);
}


//----------------------------------------------------------------
void GmEventDataHistosUA::BeginOfRunAction(const G4Run* )
{
  SetUserName( theName );

  theDataList.clear();
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  G4int index = 0;
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier);
}


//----------------------------------------------------------------
void GmEventDataHistosUA::BeginOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( evt, 0 );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::UserSteppingAction(const G4Step* aStep )
{
  /*  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndex(aStep);
  } else {
    index = 0;
  }
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );
  */

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep, 0 );
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::EndOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( evt, 0 ); // print theDataAccumulated
      }
    }
}

//----------------------------------------------------------------
void GmEventDataHistosUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
