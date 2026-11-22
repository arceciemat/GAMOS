#include "GmTrackDataBinFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"


//----------------------------------------------------------------
GmTrackDataBinFileUA::GmTrackDataBinFileUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmTrackDataBinFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << this << " GmTrackDataBinFileUA::BeginOfRunAction theOutputType " << theOutputType << G4endl;
  Init();
  OpenFileOutBin();

  theDataList.clear();
  theDataList.push_back("EventID");
  theDataList.push_back("TrackID");
  theDataList.push_back("Particle");
  theDataList.push_back("FinalPosX");
  theDataList.push_back("FinalPosY");
  theDataList.push_back("FinalPosZ");
  theDataList.push_back("FinalMomX");
  theDataList.push_back("FinalMomY");
  theDataList.push_back("FinalMomZ");
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
void GmTrackDataBinFileUA::PreUserTrackingAction(const G4Track* aTrack)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteBin( aTrack );
      } else {
	(*ite)->Initialise();
      }
    }

  theNCallsWritten++;
}

//----------------------------------------------------------------
void GmTrackDataBinFileUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }
}

//----------------------------------------------------------------
void GmTrackDataBinFileUA::PostUserTrackingAction(const G4Track* aTrack )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteBin( aTrack );
      }
    }
   theNCallsWritten++;

}

//----------------------------------------------------------------
void GmTrackDataBinFileUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
