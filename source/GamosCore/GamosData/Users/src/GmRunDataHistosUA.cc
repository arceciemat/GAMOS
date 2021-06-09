#include "GmRunDataHistosUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmRunDataHistosUA::GmRunDataHistosUA()
{
  theDataType = DTRun;

}


//----------------------------------------------------------------
void GmRunDataHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("GmRunDataHistosUA",theFilters,theClassifier);
}


//----------------------------------------------------------------
void GmRunDataHistosUA::BeginOfRunAction(const G4Run* aRun)
{
  SetUserName( theName );

  theOutputType = theName;

  theDataList.clear();
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( aRun, 0 );
      } else {
	(*ite)->Initialise();
      }
    }

  G4int index = 0;
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier);
}

//----------------------------------------------------------------
void GmRunDataHistosUA::UserSteppingAction(const G4Step* aStep )
{
  /*
  G4int index;
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
void GmRunDataHistosUA::EndOfRunAction(const G4Run* )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if( theHistos.find(0) == theHistos.end() ) BookHistos(0, theClassifier );
	(*ite)->FillHisto( (const G4Run*)0, 0 ); // print theDataAccumulated
      }
    }

  EndOfRun();
}
