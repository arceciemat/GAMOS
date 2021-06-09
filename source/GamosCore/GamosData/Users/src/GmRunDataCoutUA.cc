#include "GmRunDataCoutUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmRunDataCoutUA::GmRunDataCoutUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmRunDataCoutUA::BeginOfRunAction( const G4Run* aRun)
{
  SetUserName( theName );

  theDataList.clear();
  theDataList.push_back("RunID");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteCout( aRun, ite == theData.begin() );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmRunDataCoutUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }

}


//----------------------------------------------------------------
void GmRunDataCoutUA::EndOfRunAction( const G4Run* )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteCout( (const G4Run*)0, ite == theData.begin() );// print theDataAccumulated
      }
    }
  G4cout << G4endl;  
}
