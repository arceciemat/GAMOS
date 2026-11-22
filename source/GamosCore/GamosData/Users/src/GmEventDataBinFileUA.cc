#include "GmEventDataBinFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmEventDataBinFileUA::GmEventDataBinFileUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmEventDataBinFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << this << " GmEventDataBinFileUA::BeginOfRunAction theOutputType " << theOutputType << G4endl;
  Init();
  OpenFileOutBin();

  theDataList.clear();
  theDataList.push_back("EventID");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutBin( theFileOutBin );
    }

  WriteHeaderBin();
}

//----------------------------------------------------------------
void GmEventDataBinFileUA::BeginOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteBin( evt );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmEventDataBinFileUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }
}

//----------------------------------------------------------------
void GmEventDataBinFileUA::EndOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteBin( evt );// print theDataAccumulated
      }
    }

  theNCallsWritten++;

}

//----------------------------------------------------------------
void GmEventDataBinFileUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
