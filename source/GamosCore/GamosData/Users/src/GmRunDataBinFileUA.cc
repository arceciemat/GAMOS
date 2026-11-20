#include "GmRunDataBinFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmRunDataBinFileUA::GmRunDataBinFileUA()
{
  //--- Multithreading: workers accumulate, the master instance (with the summed data) writes
  SetRunOnMaster(true);
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmRunDataBinFileUA::BeginOfRunAction( const G4Run* aRun)
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << this << " GmRunDataBinFileUA::BeginOfRunAction theOutputType " << theOutputType << G4endl;
  if( !IsMTWorker() ) {
  Init();
  OpenFileOutBin();
  }

  theDataList.clear();
  theDataList.push_back("RunID");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();
  if( IsMTWorker() ) {
    for( std::vector<GmVData*>::const_iterator itew = theData.begin(); itew != theData.end(); itew++ ) (*itew)->Initialise();
    return;
  }

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutBin( theFileOutBin );
    }

  WriteHeaderBin();

  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteBin( aRun );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmRunDataBinFileUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }
}

//----------------------------------------------------------------
void GmRunDataBinFileUA::EndOfRunAction(const G4Run* )
{
  if( IsMTWorker() ) {
    MergeRunDataToMaster( dynamic_cast<GmVDataUser*>(GmUserAction::GetMasterUserAction(GetName())) );
    return;
  }
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteBin( (const G4Run*)0 );// print theDataAccumulated
      }
    }

  theNCallsWritten++;

  EndOfRun();
}
